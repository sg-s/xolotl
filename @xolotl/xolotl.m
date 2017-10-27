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
	OS_binary_ext % OS-specific
	xolotl_folder
	cpp_folder
	sections@cell = {};
	dyn_prop_handles % handles to dynamic properties 
	illegal_names = {'xolotl_network','compartment','conductance','controller','synapse','network'}; % list of illegal names for compartments, synpases and other objects
end  % end protected props

properties (GetAccess = protected)
	skip_hash_check@logical = false
end

properties
	controllers@cell = {}
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
			[~,cond_name,ext] = fileparts(available_conductances{i});
			if strcmp(ext,'.hpp')
				self.illegal_names{end+1} = cond_name;
				rm_this(i) = false;
			end
		end
		self.available_conductances = available_conductances(~rm_this);

		% make a list of available controllers
		available_controllers = getAllFiles(joinPath(self.cpp_folder,'controllers'));
		rm_this = true(length(available_controllers),1);
		for i = 1:length(available_controllers)
			[~,cont_name,ext] = fileparts(available_controllers{i});
			if strcmp(ext,'.hpp')
				self.illegal_names{end+1} = cont_name;
				rm_this(i) = false;
			end
		end
		self.available_controllers = available_controllers(~rm_this);


		% make a list of the available synapses
		available_synapses = getAllFiles(joinPath(self.cpp_folder,'synapses'));
		rm_this = true(length(available_synapses),1);
		for i = 1:length(available_synapses)
			[~,syn_name,ext] = fileparts(available_synapses{i});
			if strcmp(ext,'.hpp')
				self.illegal_names{end+1} = syn_name;
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
		if ~self.skip_hash_check
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
				self.controllers{i}.m = cont_m(i);
				this_channel = strrep(self.controllers{i}.channel,self.controllers{i}.compartment,'');
				this_channel(1) = [];
				self.(self.controllers{i}.compartment).(this_channel).gbar = cont_g(i);
			end 
		end

	end


end % end methods 

methods (Static)

	spiketimes = findSpikes(V)


end % end static methods
end % end classdef 