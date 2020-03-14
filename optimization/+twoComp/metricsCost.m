% this cost function can be used to fit a two compartment 
% model to data using metrics gleaned from the data


function C = metricsCost(x, data)

x.reset;

x.closed_loop = true;
x.dt = .1;
x.t_end = 10e3;
x.integrate;

V = x.integrate;

if any(isnan(V(:)))
	C = 1e3;
	return
end

C = 0;

% measure metrics in Axon
metrics = xtools.V2metrics(V(:,1),'sampling_rate',10);

C = C + 20*xtools.binCost([data.burst_period - 100, data.burst_period + 100],metrics.burst_period);
C = C + 10*xtools.binCost([data.duty_cycle - .05, data.duty_cycle + .05],metrics.duty_cycle_mean);

C = C + 20*xtools.binCost([data.n_spikes_per_burst - 1, data.n_spikes_per_burst + 3],metrics.n_spikes_per_burst_mean);


% measure minimum and maximum in soma
C = C + 5*xtools.binCost([data.min_V - 2, data.min_V + 2],min(V(:,2)));
C = C + 5*xtools.binCost([data.max_V - 2, data.max_V + 2],max(V(:,2)));

% also compare voltage traces directly
C = C + 10*xtools.voltageCost(data.V0,V(:,2),100);


% spike peaks, amplitudes, and minimum b/w spikes


[spike_amplitudes, spike_peaks, minima_bw_spikes] = twoComp.measurePDmetrics(V(:,2));


C = C + 5*xtools.binCost(data.spike_amplitude_range,nanmean(spike_amplitudes));
C = C + 5*xtools.binCost(data.spike_peaks,nanmean(spike_peaks));
C = C + 5*xtools.binCost(data.V_bw_spikes_range,nanmean(minima_bw_spikes));

% also make sure the spikes are not at very different heights
C = C + 10*xtools.binCost([0 abs(diff(data.spike_peaks))],max(spike_peaks) - min(spike_peaks));