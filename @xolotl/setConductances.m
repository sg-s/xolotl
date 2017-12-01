%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: sets all gbars of compartment's conductances to a vector of gbars
% identified by the compartment index

function setConductances(self,comp_index,gbars)

assert(length(self.getChannelsInCompartment(comp_index)) == length(gbars),'gbars must be the same length as the number of conductances in compartment')

channels 		= self.getChannelsInCompartment(comp_index);

for ii = 1:length(channels)
	self.(self.compartment_names{comp_index}).(channels{ii}).gbar = gbars(ii);
end
