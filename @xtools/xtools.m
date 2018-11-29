% the xtools class bundles some useful
% tools to convert measure metrics of
% spiketimes and voltage traces

classdef xtools

properties
end % end props

methods (Static)

	metrics = V2metrics(V,varargin);
	spiketimes = findNSpikeTimes(V,n_spikes,on_off_thresh);
	f = findNSpikes(V, on_off_thresh);
	[M, V_lim, dV_lim] = V2Matrix(V, V_lim, dV_lim);
	C = matrixCost(M1,M2)
	[neuron_metrics, phase_differences] = spiketimes2stats(varargin);
	[burst_metrics, spike_times, Ca_peaks, Ca_mins] = findBurstMetrics(V,Ca,Ca_peak_similarity, burst_duration_variability,on_off_thresh);

end % methods


end %  end classdef
