%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% add generic controllers 
% 

function addController(self,compartment,cont_id,conductance,tau_m,tau_g)

assert(any(strcmp(compartment,properties(self))),'Unknown compartment')

assert(any(strcmp(conductance,fieldnames(self.(compartment)))),'Unknown conductance')

% search for cont_id
cont_file = [];
for i = 1:length(self.available_controllers)
	if any(strfind(self.available_controllers{i},cont_id))
		cont_file = i;
		break;
	end
end
assert(~isempty(cont_file),'Which controller do you mean?')


cont_name = pathEnd(self.available_controllers{cont_file});


class_members = findCPPClassMembers(self.available_controllers{cont_file});

keyboard

S.type = cont_name;
S.channel = [compartment '_' conductance];
S.compartment = compartment;
S.tau_m = tau_m;
S.tau_g = tau_g;
S.m = 0;

self.controllers  = [self.controllers; S];

% add this to controller, if it's not already there
self.controller_headers = [self.controller_headers; self.available_controllers{cont_file}];
