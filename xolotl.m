% xolotl.m
% a MATLAB class that wraps C++ code
% it generates C++ files, compiles them, and runs them
% based on pseudo-objects that you can define within it
% 
% Srinivas Gorur-Shandilya
% see https://github.com/sg-s/xolotl
% for more information 

classdef xolotl < handle & dynamicprops

properties (SetAccess = protected)

	compartment_props 
	available_conductances
	available_synapses

end  % end protected props



properties (Access = protected)

	conductance_headers = {};
	compartment_names = {};
	synapse_headers = {};
	
end  % end protected props

properties
	dt = 50e-3;
	t_end = 5000;
	synapses
	handles
	V_clamp
end % end general props

methods 
	function self = xolotl()
		% read props from compartment.h
		cppfilename = joinPath(fileparts(which(mfilename)),'compartment.hpp');
		self.compartment_props = findCPPClassMembers(cppfilename);

		% make a list of the available conductances
		available_conductances = getAllFiles(joinPath(fileparts(which(mfilename)),'conductances'));
		rm_this = true(length(available_conductances),1);
		for i = 1:length(available_conductances)
			[~,~,ext] = fileparts(available_conductances{i});
			if strcmp(ext,'.hpp')
				rm_this(i) = false;
			end
		end
		self.available_conductances = available_conductances(~rm_this);

		% make a list of the available synapses
		available_synapses = getAllFiles(joinPath(fileparts(which(mfilename)),'synapses'));
		rm_this = true(length(available_synapses),1);
		for i = 1:length(available_synapses)
			[~,~,ext] = fileparts(available_synapses{i});
			if strcmp(ext,'.hpp')
				rm_this(i) = false;
			end
		end
		self.available_synapses = available_synapses(~rm_this);

	end

	function set.V_clamp(self,value)
		assert(isvector(value),'V_clamp must be a vector');
		value = value(:);
		assert(length(value) == self.t_end/self.dt,'V_clamp has the wrong length')
		self.V_clamp = value;
	end

	function addCompartment(self,label, varargin)
		assert(nargin > 1,'No label! You have to label every compartment')

		% check that there are as many inputs as there are compartment_props
		assert(length(self.compartment_props) ==  length(varargin),'Wrong number of compartment properties' )

		self.addprop(label);

		% blindly trust that the arguments make sense and are in the correct order
		for i = 1:length(varargin)
			self.(label).(self.compartment_props{i}) = varargin{i};
		end

		self.compartment_names = [self.compartment_names; label];

	end

	function addConductance(self,compartment,cond_id,gbar,E,m,h)
		assert(any(strcmp(compartment,properties(self))),'Unknown compartment')

		% search for cond_id
		cond_file = [];
		for i = 1:length(self.available_conductances)
			if any(strfind(self.available_conductances{i},cond_id))
				cond_file = i;
				break;
			end
		end
		assert(~isempty(cond_file),'Which conductance do you mean?')

		if nargin < 6
			m = 0;
			h = 1;
		end
		if nargin < 7
			h = 1;
		end

		cond_name = pathEnd(self.available_conductances{cond_file});

		self.(compartment).(cond_name).full_path = self.available_conductances{cond_file};
		self.(compartment).(cond_name).gbar = gbar;
		self.(compartment).(cond_name).E = E;
		self.(compartment).(cond_name).m = m;
		self.(compartment).(cond_name).h = h;

		% add this to conductance_headers, if it's not already there
		self.conductance_headers = [self.conductance_headers; self.available_conductances{cond_file}];
	end


	function addSynapse(self,syn_id,comp1,comp2,gbar)

		% fail early 
		assert(any(strcmp(comp1,self.compartment_names)),'Unknown compartment')
		assert(any(strcmp(comp2,self.compartment_names)),'Unknown compartment')
		assert(length(gbar) == 1,'gbar has the wrong size')
		assert(~isnan(gbar),'gbar cannot be NaN')

		% search for syn_id
		syn_file = [];
		for i = 1:length(self.available_synapses)
			if any(strfind(self.available_synapses{i},syn_id))
				syn_file = i;
				break;
			end
		end
		assert(~isempty(syn_file),'Which synapse do you mean?')

		syn_name = pathEnd(self.available_synapses{syn_file});

		S.type = syn_name;
		S.pre = comp1;
		S.post  = comp2;
		S.gbar = gbar;

		self.synapses  = [self.synapses; S];

		% add this to synapse_headers, if it's not already there
		self.synapse_headers = [self.synapse_headers; self.available_synapses{syn_file}];
	end


	function manipulate(self)
		% create a window to show all the traces

		[V,Ca] = self.integrate;
		time = self.dt:self.dt:self.t_end;

		self.handles.fig = figure('outerposition',[0 0 1000 900],'PaperUnits','points','PaperSize',[1000 500]); hold on
		n = length(self.compartment_names);
		for i = 1:n
			self.handles.ax(i) = subplot(n,1,i);
			self.handles.V_trace(i) = plot(self.handles.ax(i),time,V(:,i),'k');
			ylabel(self.handles.ax(i),['V_{' self.compartment_names{i} '} (mV)'] )
		end
		prettyFig('plw',1.5,'lw',1);
		
		% figure out the parameters -- one for each compartment 
		for i = 1:n
			[v,names] = struct2vec(self.(self.compartment_names{i}));
			% delete all E_, m_, and h_ parameters
			rm_this = false(length(v),1);
			for j = 1:length(v)
				if strcmp(names{j}(1:2),'m_') || strcmp(names{j}(1:2),'h_') || strcmp(names{j}(1:2),'E_') || strcmp(names{j}(1:2),'V_') 
					rm_this(j) = true;
				end
				if strcmp(names{j},'Ca_')
					rm_this(j) = true;
				end
			end
			v(rm_this) = []; names(rm_this) = [];

			% remove terminal underscores from names
			for j = 1:length(names)
				if strcmp(names{j}(end),'_')
					names{j}(end) = [];
				end
			end

			% reconstitute into a structure
			S = struct; U = struct; L = struct;
			for j = 1:length(v)
				S.(names{j}) = v(j);
				L.(names{j}) = v(j)/10;
				U.(names{j}) = v(j)*10;
			end 
			params{i} = S; lb{i} = L; ub{i} = U;
		end

		% and one more for the synapses 
		S = struct; U = struct; L = struct;
		for i = 1:length(self.synapses)
			this_name = [self.synapses(i).pre '_2_' self.synapses(i).post '_' self.synapses(i).type(1:4)];
			S.(this_name) = self.synapses(i).gbar;
			U.(this_name) = self.synapses(i).gbar*5;
			L.(this_name) = 0;
		end
		params{end+1} = S;
		lb{end+1} = L;
		ub{end+1} = U;

		% create a puppeteer instance and configure
		p = puppeteer(params,lb,ub);
		p.attachFigure(self.handles.fig);

		p.callback_function = @self.manipulateEvaluate;
		p.group_names = [self.compartment_names; 'synapses'];


		self.handles.puppeteer_object = p;

		 
	end % end manipulate 

	function manipulateEvaluate(self,parameters)
		% unpack parameters and update locally
		p = self.handles.puppeteer_object.parameters;
		assert(iscell(p),'Does not work for only one compartment because im lazy')

		for i = 1:length(p)
			S = p{i};
			f = fieldnames(S);
			
			if i <= length(self.compartment_names)
				ff = self.(self.compartment_names{i});
				% match everything we can in this compartment 
				for j = 1:length(f)
					if isfield(self.(self.compartment_names{i}),f{j})
						self.(self.compartment_names{i}).(f{j}) = S.(f{j});
					else
						% maybe one level deeper? 
						cond_name = f{j}(strfind(f{j},'_')+1:end);
						if isfield(self.(self.compartment_names{i}),cond_name)
							self.(self.compartment_names{i}).(cond_name).gbar = S.(f{j});
						end
					end
				end
			else
				% synapses?
				% blindly assume that the order we get them back is the order we have stored locally. dangerous, but it should work 
				v = struct2vec(p{i});
				assert(length(v) == length(self.synapses),'Expected this parameter set to be synapse strengths, but this does not have the same length as the synapses I have on list')
				for j = 1:length(self.synapses)
					self.synapses(j).gbar = v(j);
				end
			end
		end

		% evaluate 

		[V, Ca] = self.integrate;

		% update plots
		for i = 1:size(V,2)
			self.handles.V_trace(i).YData = V(:,i);
		end


	end

	function compile(self)
		% delete old mexBridge files
		if exist(joinPath(fileparts(which(mfilename)),'mexBridge.cpp'),'file') 
			delete(joinPath(fileparts(which(mfilename)),'mexBridge.cpp'))
		end

		% read lines from mexTemplate
		if isempty(self.V_clamp)
			cppfilename = joinPath(fileparts(which(mfilename)),'mexTemplate.cpp');
		else
			cppfilename = joinPath(fileparts(which(mfilename)),'mexTemplate_clamp.cpp');
		end
		lines = lineRead(cppfilename);

		% insert header files ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		header_files{1} = '#include "network.hpp"';
		header_files{2} = '#include "compartment.hpp"';



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
		comp_param_declarations = {}; 
		comp_param_hookups = {}; c = 1;

		for i = 1:length(self.compartment_names)
			this_comp_name = self.compartment_names{i};
			comp_param_declarations{i} = ['double *' this_comp_name '_params  = mxGetPr(prhs[' mat2str(i) ']);'];

			[v, names] = struct2vec(self.(this_comp_name));
			% append compartment name to names
			for j = 1:length(names)
				names{j} = [names{j} this_comp_name];
			end
			for j = 1:length(names)
				comp_param_hookups{c} = ['double ' names{j} '= ' this_comp_name '_params[' mat2str(j-1) '];'];
				c = c + 1;
			end
		end

		% now also make hooks for synapses 
		syn_param_declaration = {}; 
		syn_param_hookups = {}; 
		idx = length(self.compartment_names) + 1;
		syn_param_declaration = ['double * syn_params  = mxGetPr(prhs[' mat2str(idx) ']);'];

		for i = 1:length(self.synapses)
			this_syn_name = ['syn' mat2str(i) '_g'];
			
			syn_param_hookups{i} = ['double ' this_syn_name '= syn_params[' mat2str(i-1) '];'];
		end

		insert_this = [comp_param_declarations(:); syn_param_declaration; comp_param_hookups(:); syn_param_hookups(:)];

		insert_here = lineFind(lines,'//xolotl:input_declarations');
		assert(length(insert_here)==1,'Could not find insertion point for input declarations')
		lines = [lines(1:insert_here); insert_this(:); lines(insert_here+1:end)];
		
		%~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		% make the compartments here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		compartment_lines = {};
		for i = 1:length(self.compartment_names)
			this_string = ['compartment ' self.compartment_names{i} '('];
			for j = 1:length(self.compartment_props)
				this_string = [this_string self.compartment_props{j} '_' self.compartment_names{i} ','];
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
				this_channel_dec = [tc ' ' tc '_' this_comp_name '(gbar_' tc '_' this_comp_name,', E_' tc '_' this_comp_name,', m_' tc '_' this_comp_name,', h_' tc '_' this_comp_name,');'];
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
			these_channels = setdiff(fieldnames(self.(self.compartment_names{i})),self.compartment_props);
			for j = 1:length(these_channels)
				this_cond_name  = [these_channels{j} '_' this_comp_name];
				conductance_add_lines{c} = [this_comp_name '.addConductance(&' this_cond_name ');'];
				c = c+1;
			end
		end

		insert_here = lineFind(lines,'//xolotl:add_conductances_here');
		assert(length(insert_here)==1,'Could not find insertion point for conductance->cell hookups')
		lines = [lines(1:insert_here); conductance_add_lines(:); lines(insert_here+1:end)];


		% ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		% add the synapses here ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		syanpse_add_lines = {}; 
		for i = 1:length(self.synapses)
			this_type = self.synapses(i).type;
			g = mat2str(self.synapses(i).gbar);
			pre = self.synapses(i).pre;
			post = self.synapses(i).post;
			syanpse_add_lines{i} = [this_type ' syn' mat2str(i) '(syn' mat2str(i) '_g); syn' mat2str(i) '.connect(&' pre ', &' post ');'];
		end

		insert_here = lineFind(lines,'//xolotl:add_synapses_here');
		assert(length(insert_here)==1,'Could not find insertion point for synapse->cell hookups')
		lines = [lines(1:insert_here); syanpse_add_lines(:); lines(insert_here+1:end)];


		


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
		% write lines into mexBridge.cpp 
		lineWrite(joinPath(fileparts(which(mfilename)),'mexBridge.cpp'),lines);

		mex('-silent',joinPath(fileparts(which(mfilename)),'mexBridge.cpp'),'-outdir',fileparts(which(mfilename)))


	end % end compile

	function [V, Ca,I_clamp] = integrate(self)

		arguments = {};
		arguments{1} = [self.dt; self.t_end];

		% vectorize the current state 
		for i = 1:length(self.compartment_names)
			this_comp_name = self.compartment_names{i};
			arguments{i+1} = struct2vec(self.(this_comp_name));
		end

		% the next argument is the synapses
		if length(self.synapses)
			arguments{end+1} = [self.synapses.gbar];
		end

		% we need to give mexBridge the right number of arguments. 
		% so there's no way around constructing a string and running eval on it
		if isempty(self.V_clamp)
			I_clamp = [];
			eval_str = '[V,Ca] =  mexBridge(';
			for i = 1:length(arguments)
				eval_str = [eval_str 'arguments{' mat2str(i) '},'];
			end
			
			eval_str(end) = ')';
			eval_str = [eval_str ';'];
			eval(eval_str)

			V = V';
			Ca = Ca';
		else
			% add on an extra argument -- the V_clamp
			arguments{end+1} = self.V_clamp;

			eval_str = '[V,Ca,I_clamp] =  mexBridge(';
			for i = 1:length(arguments)
				eval_str = [eval_str 'arguments{' mat2str(i) '},'];
			end
			
			eval_str(end) = ')';
			eval_str = [eval_str ';'];
			eval(eval_str)

			V = V';
			Ca = Ca';
			I_clamp = I_clamp(:);
		end

	end





end % end methods 


end % end classdef 