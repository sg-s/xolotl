%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: sets all gbars of compartment's conductances to a vector of gbars
% identified by the compartment index

function setConductances(self,compartment,gbars)

	if isnumeric(compartment)
		assert(compartment <= length(self.compartment_names),'Unknown compartment')
		assert(compartment > 0,'Illegal compartment index')
		compartment = self.compartment_names{compartment};
	elseif ischar(compartment)
		assert(any(strcmp(compartment,properties(self))),'Unknown compartment')
	end

	assert(length(self.getChannelsInCompartment(compartment)) == length(gbars),'gbars must be the same length as the number of conductances in compartment')

	channels 		= self.getChannelsInCompartment(compartment);

	for ii = 1:length(channels)
		self.(compartment).(channels{ii}).gbar = gbars(ii);
	end
end
