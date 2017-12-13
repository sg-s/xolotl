%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: gets all channel names of a given compartment
% identified by the compartment name or index

function these_channels = getChannelsInCompartment(self,compartment)


if isnumeric(compartment)
	assert(compartment <= length(self.compartment_names),'Unknown compartment')
	assert(compartment > 0,'Illegal compartment index')
	compartment = self.compartment_names{compartment}; 
elseif ischar(compartment)
	assert(any(strcmp(compartment,properties(self))),'Unknown compartment')
end



these_channels = fieldnames(self.(compartment));
rm_this = false(length(these_channels),1);
for j = 1:length(these_channels)
	if any(strcmp(these_channels{j},self.compartment_props))
		rm_this(j) = true;
	end
end
these_channels(rm_this) = [];
