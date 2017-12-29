%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: set Q10s for all conductances 

function setQ10(self,q)

assert(isvector(q),'Enter Q10s as a vector')

n_channels = 0;
for i = 1:length(self.compartment_names)
	n_channels = n_channels + length(self.getConductances(i));
end


assert(length(q) == n_channels*3,['Q10 vector is of the wrong size. It should have ' oval(n_channels*3) ' elements']);

c = 1;

for i = 1:length(self.compartment_names)
	channels = self.getChannelsInCompartment(i);
	for j = 1:length(channels)
		self.(self.compartment_names{i}).(channels{j}).Q_g = q(c);
		self.(self.compartment_names{i}).(channels{j}).Q_tau_m = q(c+1);
		self.(self.compartment_names{i}).(channels{j}).Q_tau_h = q(c+2);
		c = c + 3;
	end
end
