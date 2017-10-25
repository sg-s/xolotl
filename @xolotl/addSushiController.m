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
if length(self.compartment_names) > 1
	soma = self.compartment_names(find(cellfun(@isempty, strfind(self.compartment_names,self.sections{1}))));

	assert(length(soma) == 1,' There can only be one soma compartment')
	soma = soma{1};


	for i = 1:length(self.compartment_names)
		compartment = self.compartment_names{i};

		% required fields
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

		assert(length(self.sections) == 1,'Sushi controllers only work if there is exactly one neurite that has been sectioned')

		is_section = false;
		for j = 1:length(self.sections)
			if any(strfind(compartment,self.sections{j}))
				is_section = true;
				break
			end
		end

		if is_section
			slice_idx = str2double(compartment(max(strfind(compartment,'S'))+1:end));
			if slice_idx == 1
				% upstream compartment is the soma
				upstream_compartment = soma;
			else
				% intermediatate section
				upstream_compartment = [self.sections{1} 'S' mat2str(slice_idx - 1)];
			end
			next_section = [self.sections{1} 'S' mat2str(slice_idx + 1)];
			if  any(strcmp(self.compartment_names,next_section))
				downstream_compartment = next_section;
			else
				% terminal section on dendrite
				downstream_compartment = 'NULL';
			end
		else
			% it's the soma
			downstream_compartment = [self.sections{1} 'S1'];
			upstream_compartment = 'NULL';
		end

		S.upstream_compartment = upstream_compartment;
		S.downstream_compartment = downstream_compartment;

		if isempty(self.controllers)
			self.controllers{1} = S;
		else
			self.controllers  = [self.controllers, S];
		end

	end
else
	% only one compartment -- this if for testing only
		% required fields
	compartment = self.compartment_names{1};
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

	S.downstream_compartment = 'NULL';
	S.upstream_compartment = 'NULL';

	if isempty(self.controllers)
		self.controllers{1} = S;
	else
		self.controllers  = [self.controllers, S];
	end

end


% add this to controller, if it's not already there
self.controller_headers = [self.controller_headers; self.available_controllers{cont_file}];
