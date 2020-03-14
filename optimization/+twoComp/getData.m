function data = getData()

d =  fileparts(mfilename('fullpath'));


load([d filesep 'all_PD.mat'])
V0 = all_PD;
dV0 = NaN*V0;
T = 10;
for i = 1:size(all_PD,2)
	V0(:,i) = filtfilt(ones(T,1),T,V0(:,i));
	dV0(:,i) = [NaN; diff(V0(:,i))];
end

% prepare data to be sent to cost function
data = struct;
for i = 1:size(all_PD,2)

	[spike_amplitudes, spike_peaks, minima_bw_spikes] = twoComp.measurePDmetrics(V0(:,i));

	spike_amplitude_range = [nanmin(spike_amplitudes) nanmax(spike_amplitudes)];


	smallest_spike = min(spike_peaks) - .5;


	metrics =  xtools.V2metrics(V0(:,i),'spike_threshold',smallest_spike,'sampling_rate',10);

	data(i).max_V = max(V0(:,i));
	data(i).min_V = min(V0(:,i));
	data(i).burst_period = metrics.burst_period;
	data(i).duty_cycle = metrics.duty_cycle_mean;
	data(i).V0 = V0(:,i);
	data(i).dV0 = dV0(:,i);

	data(i).spike_amplitude_range = spike_amplitude_range;

	data(i).V_bw_spikes_range = [min(minima_bw_spikes) max(minima_bw_spikes)];
	data(i).spike_peaks = [min(spike_peaks) max(spike_peaks)];

	data(i).n_spikes_per_burst = metrics.n_spikes_per_burst_mean;

end
