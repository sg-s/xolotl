%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: gets all conductances of a given compartment
% identified by the compartment name or index

function gbar = getConductances(self,compartment)

if isnumeric(compartment)
	assert(compartment <= length(self.compartment_names),'Unknown compartment')
	assert(compartment > 0,'Illegal compartment index')
	compartment = self.compartment_names{compartment}; 
elseif ischar(compartment)
	assert(any(strcmp(compartment,properties(self))),'Unknown compartment')
end


gbar = [];

[V,N] = self.serialize;
V(1) = [];
N(1) = [];

for i = 1:length(self.compartment_names)
	if strcmp(self.compartment_names{i},compartment)
		this_N = N{i};
		this_V = V{i};
		for j = 1:length(this_N)
			if any(strfind(this_N{j},'gbar'))
				gbar(end+1) = this_V(j);
			end
		end
	end
end
gbar = gbar(:);