% xolotl.m
%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% a MATLAB class that wraps C++ code
% that runs multi-compartment neuron/networks
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
	available_controllers
	available_synapses
	linked_binary@char
	compartment_names = {};

end  % end set protected props

properties (Access = protected)
	conductance_headers = {};
	controller_headers = {};
	synapse_headers = {};
	OS_binary_ext
	xolotl_folder
	cpp_folder
end  % end protected props

properties
	controllers
	dt@double = 50e-3; % ms
	t_end@double = 5000; % ms
	handles
	V_clamp
	I_ext
	closed_loop@logical = true;
	synapses
end % end general props

methods
	function self = xolotl()
		self.xolotl_folder = fileparts(fileparts(which(mfilename)));
		self.cpp_folder = joinPath(self.xolotl_folder,'c++');

		% read props from compartment.h
		cppfilename = joinPath(self.cpp_folder,'compartment.hpp');
		self.compartment_props = findCPPClassMembers(cppfilename);

		% make a list of the available conductances
		available_conductances = getAllFiles(joinPath(self.cpp_folder,'conductances'));
		rm_this = true(length(available_conductances),1);
		for i = 1:length(available_conductances)
			[~,~,ext] = fileparts(available_conductances{i});
			if strcmp(ext,'.hpp')
				rm_this(i) = false;
			end
		end
		self.available_conductances = available_conductances(~rm_this);

		% make a list of available controllers
		available_controllers = getAllFiles(joinPath(self.cpp_folder,'controllers'));
		rm_this = true(length(available_controllers),1);
		for i = 1:length(available_controllers)
			[~,~,ext] = fileparts(available_controllers{i});
			if strcmp(ext,'.hpp')
				rm_this(i) = false;
			end
		end
		self.available_controllers = available_controllers(~rm_this);


		% make a list of the available synapses
		available_synapses = getAllFiles(joinPath(self.cpp_folder,'synapses'));
		rm_this = true(length(available_synapses),1);
		for i = 1:length(available_synapses)
			[~,~,ext] = fileparts(available_synapses{i});
			if strcmp(ext,'.hpp')
				rm_this(i) = false;
			end
		end
		self.available_synapses = available_synapses(~rm_this);

		if ismac
			self.OS_binary_ext = 'mexmaci64';
		elseif ispc
			self.OS_binary_ext = 'mexw64';
		else
			self.OS_binary_ext = 'mexa64';
		end
	end

	function set.V_clamp(self,value)
		assert(isvector(value),'V_clamp must be a vector');
		value = value(:);
		assert(length(value) == self.t_end/self.dt,'V_clamp has the wrong length')
		self.V_clamp = value;
	end


	function [V, Ca,I_clamp, cond_state, syn_state, cont_state] = integrate(self)

		% check if we need to transpile or compile
		h = self.hash;
		if isempty(self.linked_binary)
			% doesn't exist -- check if we need to compile

			if exist(joinPath(self.xolotl_folder,['mexBridge' h(1:6) '.cpp']),'file') == 2
				% Ok, we have the C++ file. should we compile?
				if exist(joinPath(self.xolotl_folder,['mexBridge' h(1:6) '.' self.OS_binary_ext]),'file') == 3
					% update the linked_binary
					self.linked_binary = ['mexBridge' h(1:6) '.' self.OS_binary_ext];
				else
					self.compile;
				end
			else
				% transpile and compile
				self.transpile;
				self.compile;
			end
		else
			% disp('check that it exists')
			if exist(joinPath(self.xolotl_folder,['mexBridge' h(1:6) '.' self.OS_binary_ext]),'file') == 3
			else
				self.compile;
			end
		end

		V = [];
		Ca = [];
		I_clamp = [];
		cond_state = [];
		syn_state = [];
		cont_state = [];

		% vectorize the current state
		arguments = self.serialize;

		[~,f]=fileparts(self.linked_binary);
		f = str2func(f);
		[results{1:6}] = f(arguments{:});

		V = (results{1})';
		Ca = (results{2})';
		I_clamp = (results{3})';
		cond_state = (results{4})';
		syn_state = (results{5})';
		cont_state = (results{6})';

		% update xolotl properties based on the integration
		idx = 1;
		if self.closed_loop
			for i = 1:length(self.compartment_names)
				% update voltage and calcium
				self.(self.compartment_names{i}).V = V(end,i);
				self.(self.compartment_names{i}).Ca = Ca(end,i);

				% update the conductances
				these_channels = self.getChannelsInCompartment(i);
				for j = 1:length(these_channels)
					self.(self.compartment_names{i}).(these_channels{j}).m = cond_state(end,idx);
					idx = idx + 1;
					self.(self.compartment_names{i}).(these_channels{j}).h = cond_state(end,idx);
					idx = idx + 1;
				end
			end

			% update the synapses
			syn_g = [syn_state(end,1:2:end)];
			syn_s = [syn_state(end,2:2:end)];
			for i = 1:length(self.synapses)
				self.synapses(i).gbar = syn_g(i);
				self.synapses(i).state = syn_s(i);
			end

			% update conductances from controllers
			cont_m = [cont_state(end,1:2:end)];
			cont_g = [cont_state(end,2:2:end)];
			for i = 1:length(self.controllers)
				self.controllers(i).m = cont_m(i);
				this_channel = strrep(self.controllers(i).channel,self.controllers(i).compartment,'');
				this_channel(1) = [];
				self.(self.controllers(i).compartment).(this_channel).gbar = cont_g(i);
			end
		end

	end

	function current_trace = getCurrentTrace(self)
	  % produces traces of the currents over time
	  % for each compartment of a xolotl object

	  % perform integration using pre-set parameters
	  [V,Ca,~,cond_states,~]    = self.integrate;

	  % number of time steps
	  n_steps     = length(V(:,1));
	  % number of compartment_names
	  n_comps     = length(self.compartment_names);
	  % number of currents, indexed by compartment
	  n_currents  = zeros(n_comps,1);
	  for comp_index = 1:n_comps
	    % count number of conductances in each compartment
	    fields    = fieldnames(x.(self.compartment_names{comp_index}));
	    n_currents(comp_index)  = length(fields) - length(self.compartment_props);
	  end
	  % current traces, indexed by compartment
	  comp_currents = cell(n_comps,1);
	  curr_index_start    = 1;
	  for comp_index = 1:n_comps
	    % count up to twice the number of currents in a compartment
	    curr_index_stop   = curr_index_start + 2*n_currents(comp_index)-1;
	    % fill with n_steps x 2*n_currents matrices
	    comp_currents{comp_index} = cond_states(:,curr_index_start:curr_index_stop);
	    curr_index_start  = curr_index_stop + 1;
	  end
	  % reduce activation and inactivation variables to a single product
	  for comp_index = 1:n_comps
	    fields            = fieldnames(x.(self.compartment_names{comp_index}));
	    IGVs              = zeros(n_steps,n_comps);
	    for curr_index = 1:n_currents(comp_index)
	      % odd numbers from 1 : 2*n_currents
	      act_index       = 2*curr_index - 1;
	      % activation variables
	      act             = comp_currents{comp_index}(:,act_index);
	      % inactivation variables
	      ict             = comp_currents{comp_index}(:,act_index+1);
	      % find ionic gating variable product
	      cond_path       = x.(self.compartment_names{comp_index}).(fields{curr_index + length(self.compartment_props)}).full_path;
	      cond_file       = fileread(cond_path);
	      if strfind(cond_file,'gbar*m*m*m*m') ~= []
	        IGVs(:,curr_index) = act .* act .* act .* act .* ict;
	      elseif strfind(cond_file,'gbar*m*m*m') ~= []
	        IGVs(:,curr_index) = act .* act .* act .* ict;
	      elseif strfind(cond_file,'gbar*m*m') ~= []
	        IGVs(:,curr_index) = act .* act .* ict;
	      elseif strfind(cond_file,'gbar*m') ~= []
	        IGVs(:,curr_index) = act .* ict;
	      else
	        IGVs(:,curr_index) = act .* ict;
	      end
	    end
	    % fill with n_steps x n_currents matrices
	    comp_currents{comp_index} = IGVs;
	  end
	  % scale by maximal conductances
	  for comp_index      = 1:n_comps
	    % identify conductances in each compartment
	    fields            = fieldnames(x.(self.compartment_names{comp_index}));
	    % index by field index
	    for curr_index    = length(self.compartment_props)+1:length(fields)
	      % keep track of current by field index
	      counter         = curr_index - length(self.compartment_props);
	      % keep track of electrochemical potential
	      emf             = zeros(n_steps,1);
	      % get maximal conductance
	      gbar            = x.(self.compartment_names{comp_index}).(fields{curr_index}).gbar;
	      % scale by maximal conductance
	      comp_currents{comp_index}(:,counter) = comp_currents{comp_index}(:,counter)*gbar;
	      % check to see if the current is calcium
	      if fields{curr_index}(1:2) == 'Ca'
	        emf(:) = V(:,comp_index) - Ca(:,2*comp_index);
	      else
	        emf(:) = V(:,comp_index) - x.(self.compartment_names{comp_index}).(fields{curr_index}).E;
	      end
	      % scale by electrochemical potential
	      comp_currents{comp_index}(:,counter) = comp_currents{comp_index}(:,counter) .* emf;
	    end
	  end
	  current_trace = comp_currents;

	end

	function synaptic_trace = getSynapticTrace(self)
	  % fetches the synaptic time-traces
	  % for a xolotl object
	  % as a cell of matrices

	  % perform integration
	  [V, Ca, cond_states, syn_states, ~] = self.integrate;

	  % number of compartments
	  n_comp = length(self.compartment_names);
	  % number of synapses
	  n_syns = length(self.synapses);
	  % container for synaptic traces indexed by compartment
	  synaptic_trace = cell(n_comp,1);

	  for syndex = 1:n_syns
	    for compdex = 1:n_comp
	      % compare post-synaptic label with compartment label
	      if strcmp(self.synapses(syndex).post,self.compartment_names{compdex})
	        % append the synaptic trace to the cell
	        if strcmp(self.synapses(syndex).type,'Glutamatergic')
	          Esyn = -70;
	        elseif strcmp(self.synapses(syndex).type,'Cholinergic')
	          Esyn = -80;
	        elseif strcmp(self.synapses(syndex).type,'Electrical')
	          predex = find(strcmp(self.synapses(syndex).pre,self.compartment_names));
	          Esyn = V(:,predex);
	        else
	          disp('error: cannot find the synapse type');
	        end
	        % compute the synaptic current
	        Isyn = syn_states(:,syndex) .* ( V(:,compdex) - Esyn );
	        % append the synaptic current to the proper matrix in the cell
	        synaptic_trace{compdex}(:,end+1) = Isyn;
	      end
	    end
	  end
	  synaptic_trace
	end

end % end methods

end % end classdef
