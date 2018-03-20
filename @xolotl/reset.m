%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: resets all conductances, compartments & synapses
% 
function reset(self)

% reset all compartments 
all_compartments = self.find('compartment');
for i = 1:length(all_compartments)
	self.(all_compartments{i}).V = -60;
	self.(all_compartments{i}).Ca = self.(all_compartments{i}).Ca_in;
end

% reset all conductances
all_channels = self.find('conductance');
for i = 1:length(all_channels)
	self.set([all_channels{i} '.m'],0)
	self.set([all_channels{i} '.h'],1)
end

% reset all synapses 
for i = 1:length(self.synapses)
	self.synapses(i).s = 0;
end

% reset all controllers
all_controllers = self.find('controller');
for i = 1:length(all_controllers)
	self.set([all_controllers{i} '.m'],0)
end