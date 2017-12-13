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

classdef xolotl < handle & dynamicprops & matlab.mixin.CustomDisplay

properties (SetAccess = protected)
	compartment_props 
	available_conductances
	available_controllers
	available_synapses
	linked_binary@char
	compartment_names = {};
	OS_binary_ext % OS-specific

	conductance_headers = {};
	controller_headers = {};
	synapse_headers = {};
	xolotl_folder
	cpp_folder
	
end  % end set protected props

properties (Access = protected)
	
	sections@cell = {};

	

	dyn_prop_handles % handles to dynamic properties 
	illegal_names = {'xolotl_network','compartment','conductance','controller','synapse','network'}; % list of illegal names for compartments, synpases and other objects
end  % end protected props


properties
	debug_mode@logical = false
	skip_hash_check@logical = false
	controllers@cell = {}
	dt@double = 50e-3; % ms
	t_end@double = 5000; % ms
	handles
	V_clamp
	I_ext
	closed_loop@logical = true;
	synapses
end % end general props


methods (Access = protected)
    function displayScalarObject(self)
        url = 'https://github.com/sg-s/xolotl/';
        fprintf(['<a href="' url '">xolotl</a> object with:\n'])
        fprintf('---------------------\n')
        for i = 1:length(self.compartment_names)
        	compartment = self.compartment_names{i};
        	url = ['matlab:' inputname(1) '.' compartment];
        	fprintf(['+ <a href="' url '">' self.compartment_names{i} '</a>  \n'])
        	% now show the conductances within this channel
        	C = self.getChannelsInCompartment(i);
        	for j = 1:length(C)
        		url = ['matlab:' inputname(1) '.' compartment '.' C{j}];
        		url_str = ['<a href="' url '">' C{j} '</a>'];
        		if isa(self.(compartment).(C{j}).gbar,'function_handle')
        			g = strrep(func2str(self.(compartment).(C{j}).gbar),'@()','');
        		else
        			g = oval(self.(compartment).(C{j}).gbar);
        		end
        		if isa(self.(compartment).(C{j}).E,'function_handle')
        			E = strrep(func2str(self.(compartment).(C{j}).E),'@()','');
        		else
        			E = oval(self.(compartment).(C{j}).E);
        		end
        		info_str = [' (g=' g ', E=' E ')'];

        		% check if there is a controller for this channel
        		if ~isempty(self.controllers)
        			tc = [compartment '_' C{j}];
        			for k = 1:length(self.controllers)

        				if strcmp(self.controllers{k}.channel,tc)

        					url2 = ['matlab:' inputname(1) '.controllers{' oval(k) '}'];
        					url_str2 = ['<a href="' url2 '">' self.controllers{k}.type '</a>'];

        					info_str = [info_str '  <-- ' url_str2 ];
        				end
        			end
        		end

        		fprintf(['  > ' url_str info_str '\n'])
        	end
        	fprintf('---------------------\n')
        end

        % show controllers
        if length(self.controllers) == 0
        	fprintf('No controllers configured\n')
       	end

    end

end % end protected methods

methods 
	function self = xolotl()
		self.rebase;
	end


	function set.I_ext(self,value)
		time = self.dt:self.dt:self.t_end;
		if isscalar(value)
			if length(self.compartment_names) > 1
				error('More than one compartment. Which compartment did you want to inject current into?')
			end
			if length(value) == 1
				value = value + 0*time;
			else
				
				assert(length(time) == length(value),'I_ext has the wrong size')
			end
		else
		
	
			assert(size(value,1) == length(time),'I_ext has the wrong size')
			assert(size(value,2) == length(self.compartment_names),'I_ext has the wrong size')

		end
		self.I_ext = value;
	end

	function set.V_clamp(self,value)
		assert(isvector(value),'V_clamp must be a vector');
		value = value(:);
		assert(length(value) == self.t_end/self.dt,'V_clamp has the wrong length')
		self.V_clamp = value;
	end


	function [V, Ca,I_clamp, cond_state, syn_state, cont_state] = integrate(self, use_nocl)

		xolotl_folder = fileparts(fileparts(which(mfilename)));
		cpp_folder = joinPath(xolotl_folder,'c++');

		if nargin < 2
			use_nocl = false;
		end

		% check if we need to transpile or compile 
		if ~self.skip_hash_check
			h = self.hash;
			if isempty(self.linked_binary)
				% doesn't exist -- check if we need to compile 
				if self.debug_mode
					 cprintf('green','[DEBUG] No linked binary\n');
				end
				if exist(joinPath(xolotl_folder,['mexBridge' h(1:6) '.cpp']),'file') == 2
					if self.debug_mode
						cprintf('green','[DEBUG] C++ file exists\n');
					end
					% Ok, we have the C++ file. should we compile?
					if exist(joinPath(xolotl_folder,['mexBridge' h(1:6) '.' self.OS_binary_ext]),'file') == 3
						% update the linked_binary
						if self.debug_mode
							cprintf('green','[DEBUG] Binary exists, linking...\n');
						end
						self.linked_binary = ['mexBridge' h(1:6) '.' self.OS_binary_ext];
					else
						if self.debug_mode
							cprintf('green','[DEBUG] Compiling...\n');
						end
						self.compile;
					end
				else
					if self.debug_mode
						cprintf('green','[DEBUG] No C++ file. Transpiling and compiling...\n');
					end
					% transpile and compile
					self.transpile;
					self.compile;
				end
			else
				if self.debug_mode
					cprintf('green','[DEBUG] Checking for linked binary...\n');
				end
				if exist(joinPath(xolotl_folder,['mexBridge' h(1:6) '.' self.OS_binary_ext]),'file') == 3
					if self.debug_mode
						cprintf('green','[DEBUG] Linked binary exists.\n');
					end
				else
					if self.debug_mode
						cprintf('green','[DEBUG] Linked binary missing. Compiling...\n');
					end
					self.transpile;
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
		if self.closed_loop & nargout == 0
			% use the NOCL version
			f = [f 'NOCL'];
		elseif use_nocl 
			f = [f 'NOCL'];
		end

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
	findActivationFunction(varargin)
	findInactivationFunction(varargin)


end % end static methods
end % end classdef 