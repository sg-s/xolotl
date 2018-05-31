%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% MATLAB -> C++ transpiler
% creates a C++ file that can be compiled with mex


function transpileCore(self,in_file,out_file)


% read lines from mexTemplate
cppfilename = joinPath(self.cpp_folder,in_file);
lines = lineRead(cppfilename);

% insert network header and other critical headers
header_files{1} = joinPath(self.cpp_folder,'network.hpp');
header_files{2} = joinPath(self.cpp_folder,'compartment.hpp');
header_files{3} = joinPath(self.cpp_folder,'synapse.hpp');
header_files{4} = joinPath(self.cpp_folder,'conductance.hpp');
header_files{5} = joinPath(self.cpp_folder,'controller.hpp');
temp = self.generateHeaders; temp = temp(:);
header_files = [header_files(:); unique(temp(2:end))];

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
	input_hookups{end+1} = ['double ' names{j} ' = params[' oval(j-1) '];'];
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
for i = 1:length(self.synapses)
	synapse_add_lines{i} = ['synapses' mat2str(i) '.connect(&' self.synapse_pre{i} ', &' self.synapse_post{i} '); n_synapses ++;'];
end

insert_here = lineFind(lines,'//xolotl:add_synapses_here');
assert(length(insert_here)==1,'Could not find insertion point for synapse->cell hookups')
lines = [lines(1:insert_here); synapse_add_lines(:); lines(insert_here+1:end)];


% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% add the controllers here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% controllers work like this: every controller must have a pointer to a 
% a conductance, and in addition, must be handed over to the containing
% compartment using .addController 
% 

controller_add_lines = {};


% first, we need to add controller to the 
% channels/synapses they control and then
% we need to add them to the compartment they are in
all_controllers = self.find('controller');

for i = 1:length(all_controllers)
	% connect to synapse/conductance
	idx = max(strfind(all_controllers{i},'.'));
	cond_name = 'NULL';
	syn_name = 'NULL'; 
	if strcmp(self.get(all_controllers{i}(1:idx-1)).cpp_class_parent,'conductance')
		cond_name = strrep(all_controllers{i}(1:idx-1),'.','_');
	elseif strcmp(self.get(all_controllers{i}(1:idx-1)).cpp_class_parent,'synapse')
		syn_name = strrep(all_controllers{i}(1:idx-1),'.','_');
	else
	 	error('Controller connected to unrecognised type')
	end 

	thing_name = strrep(all_controllers{i}(1:idx-1),'.','_');
	controller_name = strrep(all_controllers{i},'.','_');
	controller_add_lines{end+1} = [controller_name '.connect(&' cond_name ',' syn_name,');'];

	% add to compartment -- this is generally the comprtment
	% that contains the conductance that this controller points to
	% but it may be overridden by self.custom_owner
	idx = [];
	if ~isempty(self.custom_owner)
		idx = find(strcmp(self.custom_owner(:,1),all_controllers{i}));
	end
	if isempty(idx)

		idx = min(strfind(all_controllers{i},'.'));
		comp_name = all_controllers{i}(1:idx-1);
	else
		comp_name = self.custom_owner{idx,2};
	end

	controller_add_lines{end+1} = [comp_name '.addController(&' controller_name ');'];
end

controller_add_lines{end+1} = ['int n_controllers = ' mat2str(length(all_controllers)) ';'];

insert_here = lineFind(lines,'//xolotl:add_controllers_here');
assert(length(insert_here)==1,'Could not find insertion point for controller hookups');
lines = [lines(1:insert_here); controller_add_lines(:); lines(insert_here+1:end)];



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
