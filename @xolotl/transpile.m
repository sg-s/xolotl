%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% MATLAB -> C++ transpiler 
% creates a C++ file that can be compiled with mex

function transpile(self)
	% delete old mexBridge files
	if exist(joinPath(self.xolotl_folder,'mexBridge.cpp'),'file') 
		delete(joinPath(self.xolotl_folder,'mexBridge.cpp'))
	end

	% read lines from mexTemplate
	if isempty(self.V_clamp)
		cppfilename = joinPath(self.cpp_folder,'mexTemplate.cpp');
	else
		cppfilename = joinPath(self.cpp_folder,'mexTemplate_clamp.cpp');
	end
	lines = lineRead(cppfilename);

	% insert header files ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	header_files{1} = '#include "c++/network.hpp"';
	header_files{2} = '#include "c++/compartment.hpp"';

	h = unique(self.conductance_headers);
	c = length(header_files)+1;
	for i = 1:length(h)
		header_files{c} = ['#include "' h{i} '"']; c = c+1;
	end

	h = unique(self.synapse_headers);
	c = length(header_files)+1;
	for i = 1:length(h)
		header_files{c} = ['#include "' h{i} '"']; c = c+1;
	end

	insert_here = lineFind(lines,'//xolotl:include_headers_here');
	assert(length(insert_here)==1,'Could not find insertion point for headers')
	lines = [lines(1:insert_here); header_files(:); lines(insert_here+1:end)];

	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% input declarations and hookups ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	[~,names] = self.serialize;

	names(1) = []; % we've already accounted for the dt, t_end

	comp_param_declarations = {}; 
	comp_param_hookups = {}; 

	 % argument names
	argin_names = [self.compartment_names; 'synapse'; 'V_clamp' ;'controller'];
	for i = 1:length(names)
		comp_param_declarations{i} = ['double *' argin_names{i} '_params  = mxGetPr(prhs[' mat2str(i) ']);'];
		these_names = names{i};
		for j = 1:length(these_names)
			comp_param_hookups{end+1} = ['double ' these_names{j} ' = ' argin_names{i} '_params[' oval(j-1) '];'];
		end
	end

	insert_this = [comp_param_declarations(:); comp_param_hookups(:)];

	insert_here = lineFind(lines,'//xolotl:input_declarations');
	assert(length(insert_here)==1,'Could not find insertion point for input declarations')
	lines = [lines(1:insert_here); insert_this(:); lines(insert_here+1:end)];
	
	%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% make the compartments here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	compartment_lines = {};
	for i = 1:length(self.compartment_names)
		this_string = ['compartment ' self.compartment_names{i} '('];
		for j = 1:length(self.compartment_props)
			this_string = [this_string  self.compartment_names{i} '_' self.compartment_props{j}  ','];
		end
		this_string(end) = ')';
		this_string = [this_string ';'];
		compartment_lines{i} = this_string;
	end

	insert_here = lineFind(lines,'//xolotl:make_compartments_here');
	assert(length(insert_here)==1,'Could not find insertion point for compartment declarations')
	lines = [lines(1:insert_here); compartment_lines(:); lines(insert_here+1:end)];

	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% make the conductances here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	conductance_lines = {}; cc = 1;
	
	for i = 1:length(self.compartment_names)
		these_channels = setdiff(fieldnames(self.(self.compartment_names{i})),self.compartment_props);
		this_comp_name = self.compartment_names{i};
		for j = 1:length(these_channels)
			tc = these_channels{j};
			
			this_channel_dec = [tc ' ' this_comp_name '_' tc '(' this_comp_name '_' tc '_gbar,' this_comp_name '_' tc '_E,' this_comp_name '_' tc '_m,' this_comp_name '_' tc '_h);'];
			conductance_lines{cc} = this_channel_dec; cc = cc + 1;
		end
	end

	insert_here = lineFind(lines,'//xolotl:make_conductances_here');
	assert(length(insert_here)==1,'Could not find insertion point for conductance declarations')
	lines = [lines(1:insert_here); conductance_lines(:); lines(insert_here+1:end)];

	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% add the conductances here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	conductance_add_lines = {}; c = 1;
	for i = 1:length(self.compartment_names)
		this_comp_name = self.compartment_names{i};
		% these_channels = setdiff(fieldnames(self.(self.compartment_names{i})),self.compartment_props);
		% this (above) doesn't work because it reorders the channel names-- 
		% we want to add the channels in C++ in the order they were added here
		these_channels = self.getChannelsInCompartment(i);

		for j = 1:length(these_channels)
			this_cond_name  = [this_comp_name  '_' these_channels{j}];
			conductance_add_lines{c} = [this_comp_name '.addConductance(&' this_cond_name ');'];
			c = c+1;
		end
	end

	insert_here = lineFind(lines,'//xolotl:add_conductances_here');
	assert(length(insert_here)==1,'Could not find insertion point for conductance->cell hookups')
	lines = [lines(1:insert_here); conductance_add_lines(:); lines(insert_here+1:end)];


	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% add the synapses here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	[~,names] = self.serialize;
	names(1) = [];
	syn_names = names{length(self.compartment_names)+1};

	syanpse_add_lines = {}; 
	for i = 1:length(self.synapses)
		this_type = self.synapses(i).type;
		g = mat2str(self.synapses(i).gbar);
		pre = self.synapses(i).pre;
		post = self.synapses(i).post;
		idx1 = (i-1)*2 + 1;
		idx2 = (i-1)*2 + 2;
		syanpse_add_lines{i} = [this_type ' syn' mat2str(i) '('  syn_names{idx1} ',' syn_names{idx2}  '); syn' mat2str(i) '.connect(&' pre ', &' post '); n_synapses ++;'];
	end

	insert_here = lineFind(lines,'//xolotl:add_synapses_here');
	assert(length(insert_here)==1,'Could not find insertion point for synapse->cell hookups')
	lines = [lines(1:insert_here); syanpse_add_lines(:); lines(insert_here+1:end)];


	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% read synapses here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	syanpse_read_lines = {};
	for i = 1:length(self.synapses)
		this_line = ['output_syn_state[i*2*n_synapses +' mat2str((i-1)*2) '] = syn' mat2str(i) '.gbar;'];
		syanpse_read_lines{end+1} = this_line;
		this_line = ['output_syn_state[i*2*n_synapses +' mat2str((i-1)*2+1) '] = syn' mat2str(i) '.s;'];
		syanpse_read_lines{end+1} = this_line;
	end

	insert_here = lineFind(lines,'//xolotl:read_synapses_here');
	assert(length(insert_here)==1,'Could not find insertion point for synapse read hookups')
	lines = [lines(1:insert_here); syanpse_read_lines(:); lines(insert_here+1:end)];


	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% add the neurons to the network  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	network_add_lines = {}; 
	for i = 1:length(self.compartment_names)
		this_comp_name = self.compartment_names{i};
		network_add_lines{i} = ['STG.addCompartment(&' this_comp_name ');'];
	end

	insert_here = lineFind(lines,'//xolotl:add_neurons_to_network');
	assert(length(insert_here)==1,'Could not find insertion point for cell->network hookup')
	lines = [lines(1:insert_here); network_add_lines(:); lines(insert_here+1:end)];

	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% if something is clamped, link up the clamping potentials   ~~~~~~~~~
	if ~isempty(self.V_clamp)
		V_clamp_idx = length(self.compartment_names) + 2;
		insert_this = ['v_drive_idx = ' mat2str((V_clamp_idx)) ';'];
		insert_here = lineFind(lines,'//xolotl:define_v_drive_idx');
		assert(length(insert_here)==1,'Could not find insertion point for telling C++ which input is V_clamp')
		lines = [lines(1:insert_here); insert_this; lines(insert_here+1:end)];
	end


	% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	% write lines into a C++ file that we can identify by hash
	h = self.hash;
	mexBridge_name = [joinPath(self.xolotl_folder,'mexBridge') h(1:6) '.cpp'];
	lineWrite(mexBridge_name,lines);

end % end transpile