%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: resets all conductances, compartments & synapses
% 
function reset(self, snap_name)

if nargin == 1

	% reset all compartments 
	% reset the Ca_average of every compartment 
	all_compartments = self.find('compartment');
	for i = 1:length(all_compartments)
		self.(all_compartments{i}).V = -60;
		self.(all_compartments{i}).Ca = self.(all_compartments{i}).Ca_in;
		self.(all_compartments{i}).Ca_average = NaN;
	end

	% reset all conductances
	all_channels = self.find('conductance');
	for i = 1:length(all_channels)
		if self.exist([all_channels{i} '.m'])
			self.set([all_channels{i} '.m'],0)
		end
		if self.exist([all_channels{i} '.h'])
			self.set([all_channels{i} '.h'],1)
		end
	end

	% reset all synapses 
	for i = 1:length(self.synapses)
		try
			self.synapses(i).s = 0;
		catch
		end
	end

	% reset all controllers
	all_controllers = self.find('controller');
	for i = 1:length(all_controllers)
		self.set([all_controllers{i} '.m'],0)
	end

else

	% check if this snap exists
	idx = find(strcmp(snap_name,{self.snapshots.name}),1,'first');
	assert(~isempty(idx),'snapshot not found')
	self.deserialize(self.snapshots(idx).V);


end
