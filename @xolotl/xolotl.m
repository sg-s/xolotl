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

classdef xolotl <  cpplab & matlab.mixin.CustomDisplay

properties (SetAccess = protected)
	linked_binary@char
	synapses
end  % end set protected props

properties (Access = protected)
	xolotl_folder
	cpp_folder
	OS_binary_ext % OS-specific
	dyn_prop_handles % handles to dynamic properties 
	illegal_names = {'xolotl_network','compartment','conductance','controller','synapse','network','x','self'}; % list of illegal names for compartments, synpases and other objects
	synapse_pre
	synapse_post

	% this stores custom owners of some objects
	% specified by put
	custom_owner
end  % end protected props


properties
	debug_mode@logical = false
	skip_hash_check@logical = false
	controllers@cell = {}

	% output delta t
	dt@double = 50e-3; % ms

	% simulation deltat t
	sim_dt@double = NaN;

	t_end@double = 5000; % ms
	handles
	closed_loop@logical = true;
	temperature@double = 11; % centigrade 
	temperature_ref@double = 11; % centigrade 
end % end general props


methods (Access = protected)
    function displayScalarObject(self)
        url = 'https://github.com/sg-s/xolotl/';
        fprintf(['\b\b\b\b\b\b\b\b\b<a href="' url '">xolotl</a> object with:\n'])
        fprintf('---------------------\n')
        compartment_names = self.find('compartment');
        for i = 1:length(compartment_names)
        	compartment = compartment_names{i};
        	url = ['matlab:' inputname(1) '.' compartment];
        	fprintf(['+ <a href="' url '">' compartment_names{i} '</a>  \n'])
        	% now show the conductances within this channel
        	C = self.(compartment).find('conductance');
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



        		fprintf(['  > ' url_str info_str '\n'])
        	end
        	fprintf('---------------------\n')
        end


    end

end % end protected methods

methods 
	function self = xolotl()
		self.rebase;
	end


	function [V, Ca,I_clamp, cond_state, syn_state, cont_state] = integrate(self,I_ext, V_clamp)

		if nargin == 2
			assert(length(I_ext) == length(self.find('compartment')),'I_ext should be a vector with an element for each compartment')
		end

		if isempty(self.linked_binary)
			h = self.hash;
			mexBridge_name = [joinPath(self.xolotl_folder,'mexBridge') h(1:6) '.cpp'];
			self.linked_binary = ['mexBridge' h(1:6) '.' self.OS_binary_ext];
		end

		% does the binary exist? 
		if exist(joinPath(self.xolotl_folder,self.linked_binary),'file') == 3
		else
			self.transpile;
			self.compile;
		end
		
		% check that sim_dt and output_dt make sense
		if isnan(self.sim_dt) || isempty(self.sim_dt)
			self.sim_dt = self.dt;
		end
		assert(isint(self.dt/self.sim_dt),'Simulation & output dt are not compatible')

		if nargout == 0 & self.closed_loop == false
			error('Are you sure you want to integrate this with no outputs and with closed_loop set to FALSE?')
		end

		V = [];
		Ca = [];
		I_clamp = [];
		cond_state = [];
		syn_state = [];
		cont_state = [];

		% vectorize the current state 
		arguments = self.serialize;

		n_comp = length(self.find('compartment'));

		switch nargin 
		case 1
			I_ext = zeros(n_comp,1);
			V_clamp = NaN(n_comp,1);
		case 2
			% only I_ext
			V_clamp = NaN(n_comp,1);
		case 3
			% ignore I_ext, since it's being clamped
			I_ext = zeros(n_comp,1);			
		end

		assert(length(I_ext) == n_comp,'Size of I_ext is incorrect')
		assert(length(V_clamp) == n_comp,'Size of V_clamp is incorrect')

		[~,f] = fileparts(self.linked_binary);
		if self.closed_loop & nargout == 0
			% use the NOCL version
			f = [f 'NOCL'];
			f = str2func(f);
			[results{1}] = f(arguments,I_ext,V_clamp);
		else
			% use the standard version
			f = str2func(f);
			[results{1:7}] = f(arguments,I_ext,V_clamp);
		end

		if self.closed_loop
			self.deserialize(results{1});
		end

		if nargout > 0

			V = (results{2})';
			Ca = (results{3})';
			I_clamp = (results{4})';
			cond_state = (results{5})';
			syn_state = (results{6})';
			cont_state = (results{7})';
		end
		

	end


end % end methods 

methods (Static)

	spiketimes = findSpikes(V)
	findActivationFunction(varargin)
	findInactivationFunction(varargin)


end % end static methods
end % end classdef 