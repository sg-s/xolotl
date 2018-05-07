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

[values,names] = self.serialize;
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


% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% ~~~~ here we call methods of the objects we made ~~~~~~~
% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

call_method_lines = {};
if ~isempty(self.call_method_data)
	for i = 1:length(self.call_method_data)
		S = self.call_method_data(i);
		call_method_lines{end+1} = [S.object '.' S.method_name '(' S.method_values ');'];
	end

	insert_here = lineFind(lines,'//xolotl:call_methods_here');
	assert(length(insert_here)==1,'Could not find insertion point for calling object methods')
	lines = [lines(1:insert_here); call_method_lines(:); lines(insert_here+1:end)];


	
end

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
for j = length(names):-1:1
	% try to figure out what the object is
	obj_idx = [];
	for i = 1:length(obj_names)
		if ~isempty(strfind(names{j},obj_names{i}))
			obj_idx = i;
		end
	end

	if isempty(obj_idx)
		output_hookups{j} = ['output_state[' mat2str(j-1) '] = params[' mat2str(j-1) '];'];
	else
		output_hookups{j} = ['output_state[' mat2str(j-1) '] = ' obj_names{obj_idx} strrep(names{j},[obj_names{obj_idx} '_'],'.') ';'];
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



% % ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% % read synapses here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

% synapse_read_lines = {};
% for i = 1:length(self.synapses)
% 	this_line = ['output_syn_state[i*2*n_synapses +' mat2str((i-1)*2) '] = syn' mat2str(i) '.gbar;'];
% 	synapse_read_lines{end+1} = this_line;
% 	this_line = ['output_syn_state[i*2*n_synapses +' mat2str((i-1)*2+1) '] = syn' mat2str(i) '.s;'];
% 	synapse_read_lines{end+1} = this_line;
% end

% insert_here = lineFind(lines,'//xolotl:read_synapses_here');
% assert(length(insert_here)==1,'Could not find insertion point for synapse read hookups')
% lines = [lines(1:insert_here); synapse_read_lines(:); lines(insert_here+1:end)];

% % ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% % read controllers here 
% % !!! TODO: there is a weakness here -- we assume that every controller
% % has a m field, and cannot have more than two states (m, gbar). need to generalize 
% % ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

% controller_read_lines = {};
% for i = 1:length(self.controllers)
% 	this_line = ['output_cont_state[i*2*n_controllers +' mat2str((i-1)*2) '] = cont' mat2str(i) '.m;'];
% 	controller_read_lines{end+1} = this_line;
% 	this_line = ['output_cont_state[i*2*n_controllers +' mat2str((i-1)*2+1) '] = cont' mat2str(i) '.get_gbar();'];
% 	controller_read_lines{end+1} = this_line;
% end

% insert_here = lineFind(lines,'//xolotl:read_controllers_here');
% assert(length(insert_here)==1,'Could not find insertion point for controllers read hookups')
% lines = [lines(1:insert_here); controller_read_lines(:); lines(insert_here+1:end)];


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

% % ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
% % if something is clamped, link up the clamping potentials   ~~~~~~~~~
% if ~isempty(self.V_clamp)
% 	comment_this = lineFind(lines,'//xolotl:disable_when_clamped');
% 	assert(length(comment_this)==1,'Could not find line to comment out for voltage clamped case')
% 	comment_this = comment_this + 1;
% 	lines{comment_this} = ['//' lines{comment_this}];

% 	V_clamp_idx = length(self.compartment_names) + 2;
% 	insert_this = ['double *V_clamp = mxGetPr(prhs[' mat2str(V_clamp_idx) ']);'];
% 	insert_here = lineFind(lines,'//xolotl:define_v_clamp_idx');
% 	assert(length(insert_here)==1,'Could not find insertion point for telling C++ which input is V_clamp')
% 	lines = [lines(1:insert_here); insert_this; lines(insert_here+1:end)];

% 	uncomment_this = lineFind(lines,'//xolotl:enable_when_clamped');
% 	assert(length(uncomment_this)>0,'Could not find line to uncomment for voltage clamped case')
% 	for i = 1:length(uncomment_this)
% 		lines{uncomment_this(i)+1} = strrep(lines{uncomment_this(i)+1},'//','');
% 	end
% end
% % you can't have both external current and voltage clamp
% assert(isempty(self.I_ext) || isempty(self.V_clamp),'cannot define both V_clamp and I_ext')
% % if there is external current, link the current to the compartments
% if ~isempty(self.I_ext)
% 	I_ext_idx = length(self.compartment_names) + 2;
% 	insert_this = ['double *I_ext = mxGetPr(prhs[' mat2str(I_ext_idx) ']);'];
% 	insert_here = lineFind(lines,'//xolotl:define_v_clamp_idx');
% 	assert(length(insert_here)==1,'Could not find insertion point for telling C++ which input is I_ext')
% 	lines = [lines(1:insert_here); insert_this; lines(insert_here+1:end)];

% 	uncomment_this = lineFind(lines,'//xolotl:enable_when_I_ext');
% 	assert(length(uncomment_this)==1,'Could not find line to uncomment for I_ext case')
% 	for i = 1:length(uncomment_this)
% 		lines{uncomment_this(i)+1} = strrep(lines{uncomment_this(i)+1},'//','');
% 	end
% end

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
end % end transpileCore