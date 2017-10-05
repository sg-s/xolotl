%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% finds channels in compartment i

function these_channels = getChannelsInCompartment(self,i)
	these_channels = fieldnames(self.(self.compartment_names{i}));
	rm_this = false(length(these_channels),1);
	for j = 1:length(these_channels)
		if any(strcmp(these_channels{j},self.compartment_props))
			rm_this(j) = true;
		end
	end
	these_channels(rm_this) = [];
end