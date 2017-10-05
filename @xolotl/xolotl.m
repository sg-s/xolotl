% xolotl.m
%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% a MATLAB class that wraps C++ code
% that runs mulit-compartment neuron/networks
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
	linked_binary@char
	compartment_names = {};
	synapses
end  % end set protected props

properties (Access = protected)
	conductance_headers = {};
	synapse_headers = {};
	OS_binary_ext
	xolotl_folder
	cpp_folder
end  % end protected props

properties
	dt@double = 50e-3; % ms
	t_end@double = 5000; % ms
	handles
	V_clamp
	closed_loop@logical = true;
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


	function [V, Ca,I_clamp, cond_state] = integrate(self)

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
		

		cond_state = [];
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
		else 
			arguments{end+1} = [];
		end

		if ~isempty(self.V_clamp)
			% add on an extra argument -- the V_clamp
			arguments{end+1} = self.V_clamp;
		end

		[~,f]=fileparts(self.linked_binary);
		f = str2func(f);
		[results{1:4}] = f(arguments{:});

		V = (results{1})';
		Ca = (results{2})';
		I_clamp = (results{3})';
		cond_state = (results{4})';

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
		end

	end


end % end methods 

end % end classdef 