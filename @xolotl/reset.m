%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: resets all conductances, compartments & synapses
% 
function [] = reset(self)

% reset all compartments 
for i = 1:length(self.compartment_names)
	% reset the voltage and the calcium
	self.(self.compartment_names{i}).V = -60;
	self.(self.compartment_names{i}).Ca = self.(self.compartment_names{i}).Ca_in;

	% reset every conductance in this compartment 
	channels = self.getChannelsInCompartment(i);

	for j = 1:length(channels)
		self.(self.compartment_names{i}).(channels{j}).m = 0;
		self.(self.compartment_names{i}).(channels{j}).h = 1;
	end
end


% reset all synapses
for i = 1:length(self.synapses)
	self.synapses(i).state = 0;
end