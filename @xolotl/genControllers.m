%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% generate controllers for every conductance
% in "compartment", picking timescales so that
% the ratio of conductances is predicted to be
% maintained
% usage:
% x.genControllers('compartment_name','controller_type',tau_m_ref,tau_g);


function genControllers(self,compartment,controller_type,tau_m_ref,tau_g)

assert(isnumeric(tau_m_ref),'tau_m_ref should be a number')
assert((tau_m_ref>0),'tau_m_ref should be > 0')
assert(isnumeric(tau_g),'tau_g should be a number')
assert((tau_g>0),'tau_g should be > 0')
assert(any(strcmp(compartment,properties(self))),'Unknown compartment')	

% search for cont_id
cont_file = [];
for i = 1:length(self.available_controllers)
	if any(strfind(self.available_controllers{i},controller_type))
		cont_file = i;
		break;
	end
end
assert(~isempty(cont_file),'Which controller do you mean?')

original_state = self.closed_loop;
self.closed_loop = false;
[V,Ca] = self.integrate;
self.closed_loop = original_state;

% measure the Calcium target


for i = 1:length(self.compartment_names)
	if strcmp(self.compartment_names{i},compartment)
		channels = self.getChannelsInCompartment(i);

		self.(compartment).Ca_target = mean(Ca(:,i));

		gbar = self.getConductances(compartment);
		all_tau = (tau_m_ref.*gbar(1))./gbar;

		for j = 1:length(channels)
			if ~strcmp(channels{j},'Leak')
				self.addController(compartment,channels{j},controller_type,all_tau(j),tau_g,1);
			end
		end

	end
end
self.transpile;
self.compile;