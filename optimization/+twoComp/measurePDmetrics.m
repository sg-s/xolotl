function [spike_amplitudes, spike_peaks, minima_bw_spikes] = measurePDmetrics(V)

[spike_peaks,locs] = findpeaks(V,'MinPeakProminence',5);

spike_amplitudes = NaN*locs;
minima_bw_spikes = NaN*locs;

for j = 2:length(locs)
	if locs(j) - locs(j-1) > 1000
		continue
	end

	minima_bw_spikes(j) = min(V(locs(j-1):locs(j)));

	spike_amplitudes(j) =  abs(spike_peaks(j) - minima_bw_spikes(j));

end

if isempty(spike_amplitudes)
	spike_amplitudes = NaN;
end

if isempty(spike_peaks)
	spike_peaks = NaN;
end

if isempty(minima_bw_spikes)
	minima_bw_spikes = NaN;
end
