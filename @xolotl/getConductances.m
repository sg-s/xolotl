%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% gets all conductances of a given compartment, 
% identified by the compartment name 

function gbar = getConductances(self,compartment)

assert(any(strcmp(compartment,properties(self))),'Unknown compartment')

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