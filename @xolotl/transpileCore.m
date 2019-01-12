%{   
             _       _   _ 
  __  _____ | | ___ | |_| |
  \ \/ / _ \| |/ _ \| __| |
   >  < (_) | | (_) | |_| |
  /_/\_\___/|_|\___/ \__|_|

### transpileCore

**Syntax**

```matlab
x.transpileCore(in_file,out_file)
```

**Description**

method that writes C++ bridge code to set up
and integrate your model based on the 
objects configured in the xolotl tree. This is 
internally called by xolotl.transpile()

Do not call this method. It is not meant 
to be user accessible. 

!!! info "See Also"
    ->xolotl.transpile

%}

function transpileCore(self,in_file,out_file)


if self.verbosity > 0
	disp(['[INFO] starting transpile with hash: ' self.hash])
end

% read lines from mexTemplate
cppfilename = joinPath(self.cpp_folder,in_file);
lines = lineRead(cppfilename);

% insert network header and other critical headers
header_files{1} = joinPath(self.cpp_folder,'network.hpp');
header_files{2} = joinPath(self.cpp_folder,'compartment.hpp');
header_files{3} = joinPath(self.cpp_folder,'synapse.hpp');
header_files{4} = joinPath(self.cpp_folder,'conductance.hpp');
header_files{5} = joinPath(self.cpp_folder,'mechanism.hpp');
temp = self.generateHeaders; temp = temp(:);
temp(cellfun(@isempty,temp)) = [];
header_files = [header_files(:); unique(temp(:))];


for i = 1:length(header_files)
	header_files{i} = strcat('#include "',header_files{i}, '"'); 
end

insert_here = lineFind(lines,'//xolotl:include_headers_here');
assert(length(insert_here)==1,'Could not find insertion point for headers')
lines = [lines(1:insert_here); header_files(:); lines(insert_here+1:end)];


% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% input declarations and hookups ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% this section of transpile generates a list of unique variable names that identifies all parameters and variables in the xolotl object using x.serialize, and then wires up arrays from matlab that correspond to these values to named variables in C++. so you can expect every variable to exist in your C++ workspace, where the names are defined in x.serialize

[values,names,~,real_names] = self.serialize;
input_hookups{1} = ['double * params  = mxGetPr(prhs[0]);'];

for j = 1:length(names)
	input_hookups{end+1} = ['double ' names{j} ' = params[' mat2str(j-1) '];'];
end
input_hookups{end+1} = ['int param_size = ' mat2str(length(names)) ';'];
insert_here = lineFind(lines,'//xolotl:input_declarations');
assert(length(insert_here)==1,'Could not find insertion point for input declarations')
lines = [lines(1:insert_here); input_hookups(:); lines(insert_here+1:end)];


% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% make all the C++ objects 
[constructors, class_parents, obj_names] = self.generateConstructors;


insert_here = lineFind(lines,'//xolotl:insert_constructors');
assert(length(insert_here)==1,'Could not find insertion point for object constructors')
lines = [lines(1:insert_here); constructors(:); lines(insert_here+1:end)];

% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% ~~ output declarations and hookups ~~
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

% sort all objects by length of object name
% this is to make sure we find the longest object
% string in names later on 
[~,sort_idx] = sort(cellfun(@(x) length(x), obj_names));
obj_names = obj_names(sort_idx);
class_parents = class_parents(sort_idx);


output_hookups = {};
for j = length(real_names):-1:1
	% try to figure out what the object is
	% that contains this variable 

	last_dot_idx = max(strfind(real_names{j},'.'));

	if isempty(last_dot_idx)
		output_hookups{j} = ['output_state[' mat2str(j-1) '] = params[' mat2str(j-1) '];'];
	else
		containing_obj = real_names{j}(1:last_dot_idx-1);
		prop_to_read = strrep(real_names{j},containing_obj,'');
		containing_obj = strrep(containing_obj,'.','_');
		containing_obj = strrep(containing_obj,'(','');
		containing_obj = strrep(containing_obj,')','');
		output_hookups{j} = ['output_state[' mat2str(j-1) '] = ' containing_obj prop_to_read ';'];
	end
end


insert_here = lineFind(lines,'//xolotl:read_state_back');
assert(length(insert_here)==1,'Could not find insertion point for input declarations')
lines = [lines(1:insert_here); output_hookups(:); lines(insert_here+1:end)];


%% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% here we hook up the channels to the compartments they
% should be in. we need to add them in the order they occur in serialize
% so let's make sure we do that
channel_hookups = {};
all_channels = self.find('conductance');

for i = 1:length(all_channels)
	idx = strfind(all_channels{i},'.');
	comp_name = all_channels{i}(1:idx-1);
	channel_name = strrep(all_channels{i},'.','_');
	channel_hookups{end+1} = [comp_name '.addConductance(&' channel_name ');'];

end


insert_here = lineFind(lines,'//xolotl:add_conductances_here');
assert(length(insert_here)==1,'Could not find insertion point for conductance->cell hookups')
lines = [lines(1:insert_here); channel_hookups(:); lines(insert_here+1:end)];


% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% ~~~~~~~~~~~~~~ add the synapses here  ~~~~~~~~~~~~~~~~~
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


synapse_add_lines = {};
for i = length(self.synapses):-1:1
	syn_name = strrep(self.synapses(i).syn_name,'.','_');
	synapse_add_lines{i} = [syn_name '.connect(&' self.synapses(i).pre_synapse ', &' self.synapses(i).post_synapse '); n_synapses ++; all_synapses.push_back(&' syn_name ');'];
end


insert_here = lineFind(lines,'//xolotl:add_synapses_here');
assert(length(insert_here)==1,'Could not find insertion point for synapse->cell hookups')
lines = [lines(1:insert_here); synapse_add_lines(:); lines(insert_here+1:end)];


% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% add the mechanisms here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% mechanisms work like this: every mechanism must have a pointer to a 
% a conductance, and in addition, must be handed over to the containing
% compartment using .addMechanism 
% 

mechanism_add_lines = {};


% first, we need to add mechanism to the 
% channels/synapses they control and then
% we need to add them to the compartment they are in
all_mechanisms = self.find('mechanism');

for i = 1:length(all_mechanisms)
	% connect to synapse/conductance
	idx = max(strfind(all_mechanisms{i},'.'));
	cond_name = 'NULL';
	syn_name = 'NULL'; 

	parent_name = strrep(all_mechanisms{i}(1:idx-1),'.','_');
	mechanism_name = strrep(all_mechanisms{i},'.','_');

	mechanism_add_lines{end+1} = [mechanism_name '.connect(&' parent_name ');'];


end

mechanism_add_lines{end+1} = ['int n_mechanisms = ' mat2str(length(all_mechanisms)) ';'];

insert_here = lineFind(lines,'//xolotl:add_mechanisms_here');
assert(length(insert_here)==1,'Could not find insertion point for mechanism hookups');
lines = [lines(1:insert_here); mechanism_add_lines(:); lines(insert_here+1:end)];



% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% add the neurons to the network  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
network_add_lines = {};
compartment_names = sort(self.find('compartment'));
for i = 1:length(compartment_names)
	network_add_lines{i} = ['xolotl_network.addCompartment(&' compartment_names{i} ');'];
end

insert_here = lineFind(lines,'//xolotl:add_neurons_to_network');
assert(length(insert_here)==1,'Could not find insertion point for cell->network hookup')
lines = [lines(1:insert_here); network_add_lines(:); lines(insert_here+1:end)];


% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% write lines into a C++ file that we can identify by hash


if ispc
	% covert all \ to /
	for i = 1:length(lines)
		lines{i} = strrep(lines{i},'\','/');
	end
end

mexBridge_name = joinPath(self.xolotl_folder, out_file);
lineWrite(mexBridge_name,lines);
