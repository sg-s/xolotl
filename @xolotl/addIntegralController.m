%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: add integral controller to compartment, conductance 
% example:
% x.addIntegralController('AB','NaV',100,100)

function addIntegralController(self,compartment,conductance,tau_m,tau_g)

assert(any(strcmp(compartment,properties(self))),'Unknown compartment')

assert(any(strcmp(conductance,fieldnames(self.(compartment)))),'Unknown conductance')

% search for cont_id
cont_file = [];
for i = 1:length(self.available_controllers)
	if any(strfind(self.available_controllers{i},'IntegralController'))
		cont_file = i;
		break;
	end
end
assert(~isempty(cont_file),'Which controller do you mean?')


cont_name = pathEnd(self.available_controllers{cont_file});

% required fields
S.type = cont_name;
S.channel = [compartment '_' conductance];
S.compartment = compartment;

cpp_path = strrep(self.available_controllers{cont_file},self.xolotl_folder,'');
if strcmp(cpp_path(1),oss)
	cpp_path(1) = [];
end

S.cpp_path = cpp_path;

% controller-specfic parameters and variables
S.tau_m = tau_m;
S.tau_g = tau_g;
S.m = 0;


if isempty(self.controllers)
	self.controllers{1} = S;
else
	self.controllers  = [self.controllers, S];
end


% add this to controller, if it's not already there
self.controller_headers = [self.controller_headers; S.cpp_path];
