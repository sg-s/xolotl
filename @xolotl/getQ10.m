%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: get Q10s for all conductances 

function q = getQ10(self)

n_channels = 0;
for i = 1:length(self.compartment_names)
	n_channels = n_channels + length(self.getConductances(i));
end

q = NaN(n_channels*3,1);

c = 1;

for i = 1:length(self.compartment_names)
	channels = self.getChannelsInCompartment(i);
	for j = 1:length(channels)
		q(c) = self.(self.compartment_names{i}).(channels{j}).Q_g;
		q(c+1) = self.(self.compartment_names{i}).(channels{j}).Q_tau_m;
		q(c+2) = self.(self.compartment_names{i}).(channels{j}).Q_tau_h;
		c = c + 3;
	end
end
