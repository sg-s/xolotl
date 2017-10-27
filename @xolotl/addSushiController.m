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

function addSushiController(self,conductance,soma_comp, section_id, tau_m,tau_g,Alpha,Beta,Gamma)

assert(any(strcmp(soma_comp,self.compartment_names)),'Unknown soma compartment')
assert(any(strcmp(section_id,self.sections)),'Unknown section id')

% search for cont_id
cont_file = lineFind(self.available_controllers,'Sushi');
cont_name = pathEnd(self.available_controllers{cont_file});


% to do -- check that there is no other controller type here that can screw things up 

% add to soma 
% required fields
S.type = cont_name;
S.channel = [soma_comp '_' conductance];
S.compartment = soma_comp;
S.cpp_path = self.available_controllers{cont_file};

% controller-specfic parameters and variables
S.tau_m = tau_m;
S.tau_g = tau_g;
S.Alpha = Alpha;
S.Beta = Beta;
S.Gamma = Gamma;
S.m = 0;

S.downstream_compartment = [section_id 'S1'];
S.upstream_compartment = 'NULL';

if isempty(self.controllers)
	self.controllers{1} = S;
else
	self.controllers  = [self.controllers, S];
end

for i = 1:length(self.compartment_names)
	if strfind(self.compartment_names{i},section_id)

		compartment = self.compartment_names{i};
		slice_idx = str2double(compartment(max(strfind(compartment,'S'))+1:end));

		clear S
		S.type = cont_name;
		S.channel = [compartment '_' conductance];
		S.compartment = compartment;
		S.cpp_path = self.available_controllers{cont_file};

		% controller-specfic parameters and variables
		S.tau_m = tau_m;
		S.tau_g = tau_g;
		S.Alpha = Alpha;
		S.Beta = Beta;
		S.Gamma = Gamma;
		S.m = 0;

		if slice_idx == 1
			% upstream compartment is the soma
			upstream_compartment = soma_comp;
		else
			% intermediatate section
			upstream_compartment = [section_id 'S' mat2str(slice_idx - 1)];
		end
		next_section = [section_id 'S' mat2str(slice_idx + 1)];
		if  any(strcmp(self.compartment_names,next_section))
			downstream_compartment = next_section;
		else
			% terminal section on dendrite
			downstream_compartment = 'NULL';
		end

		% add to list of controllers
		S.upstream_compartment = upstream_compartment;
		S.downstream_compartment = downstream_compartment;


		self.controllers  = [self.controllers, S];


	end
end

% add this to controller, if it's not already there
self.controller_headers = [self.controller_headers; self.available_controllers{cont_file}];
