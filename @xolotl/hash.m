%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% hash function for xolotl. requires Java;
% but falls back to system md5 without Java
% if you have neither (running headless on Windows)
% you're shit out of luck

function [h] = hash(self)
	% hash all compartments 
	h = {}; idx = 1;
	for i = 1:length(self.compartment_names)
		these_channels = self.getChannelsInCompartment(i);
		for j = 1:length(these_channels)
			h{j} = self.(self.compartment_names{i}).(these_channels{j}).full_path; idx = idx + 1;
		end
	end
	if ~isempty(self.synapses)
		h{end+1} = ([{self.synapses.type} {self.synapses.pre} {self.synapses.post}]);
	end

	% include everything in serialize, just to be safe
	[~,names] = self.serialize;
	h = [h names];

	if usejava('jvm')
		h = dataHash(h);
	else
		save('temp.mat','h');
		h = md5('temp.mat');
		delete('temp.mat');
	end
end