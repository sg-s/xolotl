%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% add sushi-belt integral controller 
% a sushi belt controller only makes sense
% when added to multiple compartments
% that are ideally part of a neurite
% representing a dendrite 

function addSushiController(self,conductance,tau_m,tau_g,Alpha,Beta,Gamma)

assert(any(strcmp(conductance,fieldnames(self.(compartment)))),'Unknown conductance')

% search for cont_id
cont_file = [];
for i = 1:length(self.available_controllers)
	if any(strfind(self.available_controllers{i},'SushiController'))
		cont_file = i;
		break;
	end
end

% to do -- check that there is no other controller type here that can screw things up 

cont_name = pathEnd(self.available_controllers{cont_file});

% add a controller to every compartment 


S.type = cont_name;
S.channel = [compartment '_' conductance];
S.compartment = compartment;
S.tau_m = tau_m;
S.tau_g = tau_g;
S.Alpha = Alpha;
S.m = 0;

self.controllers  = [self.controllers; S];

% add this to controller, if it's not already there
self.controller_headers = [self.controller_headers; self.available_controllers{cont_file}];
