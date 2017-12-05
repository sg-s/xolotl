%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: generate integral controllers for every conductance
% in "compartment", picking timescales so that
% the ratio of conductances is predicted to be
% maintained
% usage:
% x.genControllers('compartment_name',tau_m_ref,tau_g);


function genIntegralControllers(self,compartment,tau_m_ref,tau_g)

if nargin ~= 4
	warning('genControllers(self,compartment,tau_m_ref,tau_g)')
	error('Wrong number of input arguments')
end

assert(isnumeric(tau_m_ref),'tau_m_ref should be a number')
assert((tau_m_ref>0),'tau_m_ref should be > 0')
assert(isnumeric(tau_g),'tau_g should be a number')
assert((tau_g>0),'tau_g should be > 0')
assert(any(strcmp(compartment,properties(self))),'Unknown compartment')	

% search for cont_id
cont_file = [];
for i = 1:length(self.available_controllers)
	if any(strfind(self.available_controllers{i},'IntegralController'))
		cont_file = i;
		break;
	end
end
assert(~isempty(cont_file),'Which controller do you mean?')

original_state = self.closed_loop;
original_t_end = self.t_end;
original_dt = self.dt;

self.closed_loop = false;
self.t_end = 20e3;
self.dt = 100e-3;
z = floor(10e3/100e-3);
[V,Ca] = self.integrate;

self.closed_loop = original_state;
self.dt = original_dt;
self.t_end = self.t_end;

re_compile = false;

for i = 1:length(self.compartment_names)
	if strcmp(self.compartment_names{i},compartment)
		channels = self.getChannelsInCompartment(i);

		self.(compartment).Ca_target = mean(Ca(z:end,i));

		gbar = self.getConductances(compartment);
		all_tau = round((tau_m_ref.*gbar(1))./gbar);

		for j = 1:length(channels)
			if ~strcmp(channels{j},'Leak')
				% check if there is already an integral controller for this
	    		if ~isempty(self.controllers)
	    			tc = [compartment '_' channels{j}];
	    			add_controller = true;
	    			for k = 1:length(self.controllers)
	    				if strcmp(self.controllers{k}.channel,tc)
	    					% controller already exists -- modify it
	    					self.controllers{k}.tau_m = all_tau(j);
	    					self.controllers{k}.tau_g = tau_g;
	    					add_controller = false;
	    				end
	    			end
	    			if add_controller
	    				re_compile = true;
	    				self.addIntegralController(compartment,channels{j},all_tau(j),tau_g);
	    			end
	        	end
	        	% reset all the conductances to have strength 0 
				self.(compartment).(channels{j}).gbar = 0;
			end
			
		end

	end
end

if re_compile
	self.transpile;
	self.compile;
end



