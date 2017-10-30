%              _       _   _ 
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% help: find spikes in voltage trace(s)
% 
function spiketimes = findSpikes(V)
	n = size(V,2);
	spiketimes = NaN(1e3,n);
	for j = 1:n
		this_V = V(:,j);
		[ons, offs] = computeOnsOffs(this_V > 0);
		c = 1;
		for i = 1:length(ons)
			[~,idx] = max(this_V(ons(i):offs(i)));
			spiketimes(c,j) = ons(i) + idx;
			c = c + 1;
		end
	end
	if n > 1
		z = find(any(~isnan(spiketimes)'),1,'last');
		spiketimes = spiketimes(1:z,:);
	else
		z = c;
		spiketimes = spiketimes(1:z-1);
	end
	
end