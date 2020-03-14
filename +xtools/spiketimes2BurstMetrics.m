
function metrics = spiketimes2BurstMetrics(spiketimes, varargin)

corelib.assert(isvector(spiketimes),'spiketimes must be a vector')

spiketimes = spiketimes(:);


metrics.burst_period_mean = NaN;
metrics.burst_period_std = NaN;
metrics.duty_cycle_mean = NaN;
metrics.duty_cycle_std = NaN;
metrics.n_spikes_per_burst_mean = NaN;
metrics.n_spikes_per_burst_std = NaN;

MaxISI = .3; % anything over this is considered a inter-burst interval
min_spikes_per_burst = 2;


isis = diff(spiketimes);
burst_starts =  find(circshift(isis > MaxISI,1));
burst_ends = find(isis > MaxISI);

if isempty(burst_starts)
	return
end

if isempty(burst_ends)
	return
end

% can't have a burst end before a burst start
burst_ends(burst_ends<burst_starts(1)) = [];

if isempty(burst_ends)
	return
end

% can't have a burst start after the last burst end
burst_starts(burst_starts>burst_ends(end)) = [];

if length(burst_starts) ~= length(burst_ends)
	keyboard
end

n_spikes_per_burst = burst_ends - burst_starts;


burst_starts(n_spikes_per_burst<min_spikes_per_burst) = [];
burst_ends(n_spikes_per_burst<min_spikes_per_burst) = [];
n_spikes_per_burst(n_spikes_per_burst<min_spikes_per_burst) = [];


if isempty(burst_starts)
	return
end

if isempty(burst_ends)
	return
end


% convert to real time 
burst_starts = spiketimes(burst_starts);
burst_ends = spiketimes(burst_ends);
burst_periods = [diff(burst_starts); NaN];

burst_durations = burst_ends - burst_starts;

metrics.burst_period_mean = nanmean(burst_periods);
metrics.burst_period_std = nanstd(burst_periods);

metrics.duty_cycle_mean = nanmean(burst_durations./burst_periods);
metrics.duty_cycle_std = nanstd(burst_durations./burst_periods);

metrics.n_spikes_per_burst_mean = nanmean(n_spikes_per_burst);
metrics.n_spikes_per_burst_std = nanstd(n_spikes_per_burst);