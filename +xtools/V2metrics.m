%{

;;     ;; ;;;;;;;;  ;;;;;;;   ;;;;;;;  ;;        ;;;;;;
 ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
  ;; ;;      ;;    ;;     ;; ;;     ;; ;;       ;;
   ;;;       ;;    ;;     ;; ;;     ;; ;;        ;;;;;;
  ;; ;;      ;;    ;;     ;; ;;     ;; ;;             ;;
 ;;   ;;     ;;    ;;     ;; ;;     ;; ;;       ;;    ;;
;;     ;;    ;;     ;;;;;;;   ;;;;;;;  ;;;;;;;;  ;;;;;;

### V2metrics

**Syntax**

```matlab
options = xtools.V2metrics
metrics = xtools.V2metrics(V)
metrics = xtools.V2metrics(V, options)
metrics = xtools.V2metrics(V, 'PropertyName', PropertyValue, ...)
```

**Description**

Computes metrics from a raw time series of voltage, 
which can be experimental or simulated data.

If called without arguments or outputs, a struct
containing fields for all optional arguments, `options`, 
is created.

`V2metrics` can be called using a struct to specify 
options, or with individual options specified as name, value keyword pairs. Options with a `NaN` value are ignored
and the default option value is used instead.

| Option Name | Default Value | Units |
| ----------- | ------------- |
| `sampling_rate` | 20 | 1/ms |
| `ibi_thresh` | 300 | ms |
| `spike_threshold` | 0 | mV |
| `debug` | false | |

| Metric Name | Units |
| ----------- | ----- |
| `firing_rate` | NaN | ? |
| `burst_period` | NaN | ? |
| `ibi_mean` | NaN | ? |
| `ibi_std` | NaN | ? |
| `isi_std` | NaN | ? |
| `burst_period_std` | NaN | ? |
| `isi_std` | NaN | ? |
| `duty_cycle_mean` | NaN | ? |
| `n_spikes_per_burst_mean` | NaN | ? |
| `n_spikes_per_burst_std` | NaN | ? |
| `min_V_mean` | NaN | ? |
| `min_V_std` | NaN | ? |
| `min_V_in_burst_mean` | NaN | ? |
| `min_V_in_burst_std` | NaN | ? |
| `spike_peak_mean`  | NaN | ? |
| `spike_peak_std` | NaN | ? |


!!! info "See Also"
    ->xtools.findNSpikes
	->xtools.findNSpikeTimes
	->xtools.findBurstMetrics
	LeMasson G, Maex R (2001) Introduction to equation solving and parameter fitting. In: De Schutter E (ed) Computational Neuroscience: Realistic Modeling for Experimentalists. CRC Press, London pp 1–21


%}

function metrics = V2metrics(V, varargin)


metrics = orderfields(struct('firing_rate',NaN,'burst_period',NaN,'ibi_mean',NaN,'ibi_std',NaN,'isi_mean',NaN,'burst_period_std',NaN,'isi_std',NaN,'duty_cycle_mean',NaN,'duty_cycle_std',NaN,'n_spikes_per_burst_mean',NaN,'n_spikes_per_burst_std',NaN,'min_V_mean',NaN,'min_V_std',NaN,'min_V_in_burst_mean',NaN,'min_V_in_burst_std',NaN,'spike_peak_mean',NaN,'spike_peak_std',NaN,'ibi_thresh',NaN));

% options and defaults
options.sampling_rate = 20; % samples per millisecond
options.ibi_thresh = 3e2; % inter-burst interval in ms
options.spike_threshold = 0; % mV
options.debug = false;

if nargout && ~nargin
	varargout{1} = options;
    return
end

% validate and accept options
if iseven(length(varargin))
	for ii = 1:2:length(varargin)-1
	temp = varargin{ii};
    if ischar(temp)
    	if ~any(find(strcmp(temp,fieldnames(options))))
    		disp(['Unknown option: ' temp])
    		disp('The allowed options are:')
    		disp(fieldnames(options))
    		error('UNKNOWN OPTION')
    	else
    		options.(temp) = varargin{ii+1};
    	end
    end
end
elseif isstruct(varargin{1})
	% should be OK...
	options = varargin{1};
else
	error('Inputs need to be name value pairs')
end


assert(isvector(V),'V should be a vector')
V = V(:);
assert(~any(isnan(V)),'V cannot have NaNs')
assert(~any(isinf(V)),'V cannot have Inf')
assert((isreal(V)),'V cannot be complex')

% find spikes in voltage trace
n_spikes = xtools.findNSpikes(V);
spiketimes = xtools.findNSpikeTimes(V,n_spikes);

spiketimes(spiketimes > length(V)) = [];

if n_spikes == 0
	metrics.firing_rate = 0;
	return
end

% compute firing rate
metrics.firing_rate = (1e3*n_spikes)/(length(V)/options.sampling_rate);

% find ISI
metrics.isi_mean = mean(diff(spiketimes))/options.sampling_rate;
metrics.isi_std = std(diff(spiketimes))/options.sampling_rate;


% measure some statistics about the extrema of
% the voltage trace
metrics.min_V_mean = min(V);


% measure spike height stats
metrics.spike_peak_mean = mean(V(spiketimes));
metrics.spike_peak_std = std(V(spiketimes));




% begin burst-related metrics


% find burst starts and stops
metrics.ibi_thresh = options.ibi_thresh;
ibi = options.ibi_thresh*options.sampling_rate;
burst_ends = spiketimes(find(diff(spiketimes) > ibi));

burst_starts = find(diff(spiketimes) > ibi) + 1;
burst_starts(burst_starts>length(spiketimes)) = [];
burst_starts = spiketimes(burst_starts);

% find IBI statistics
all_isis = diff(spiketimes);
metrics.ibi_mean = mean(all_isis(all_isis > ibi))/options.sampling_rate;
metrics.ibi_std = std(all_isis(all_isis > ibi))/options.sampling_rate;

if options.debug
	figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
	plot(V,'k')
	plot(burst_ends,V(burst_ends),'ro')
	plot(burst_starts,V(burst_starts),'go')
end

if length(burst_starts) < 3
	return
end

if length(burst_ends) < 3
	return
end

metrics.burst_period = mean(diff(burst_ends))/options.sampling_rate;
metrics.burst_period_std = std(diff(burst_ends))/options.sampling_rate;


% align burst_starts and burst_stops so we
% have one for every identified burst
if burst_starts(1) > burst_ends(1)
	burst_ends(1) = [];
end

burst_starts(burst_starts > burst_ends(end)) = [];

if length(burst_starts) ~= length(burst_ends)
	disp('mismatched starts and stops')
	keyboard
end

% we measure these things on a per-burst
% basis and then average across all bursts
all_dc = NaN*burst_starts;
all_n_spikes = NaN*burst_starts;
all_min_V = NaN*burst_starts;
all_min_V_in_burst = NaN*burst_starts;

for i = 2:length(all_dc)
	all_n_spikes(i) = sum(spiketimes >= burst_starts(i) & spiketimes <= burst_ends(i));

	all_min_V_in_burst(i) = min(V(burst_starts(i):burst_ends(i)));
	all_min_V(i) = min(V(burst_ends(i-1):burst_ends(i)));

	all_dc(i) = (burst_ends(i) - burst_starts(i))/(burst_ends(i) - burst_ends(i-1));
end


metrics.min_V_in_burst_mean = mean(all_min_V_in_burst(2:end));
metrics.min_V_in_burst_std = std(all_min_V_in_burst(2:end));

metrics.min_V_mean = mean(all_min_V(2:end));
metrics.min_V_std = std(all_min_V(2:end));

metrics.duty_cycle_mean = mean(all_dc(2:end));
metrics.duty_cycle_std = std(all_dc(2:end));

metrics.n_spikes_per_burst_mean = mean(all_n_spikes(2:end));
metrics.n_spikes_per_burst_std = std(all_n_spikes(2:end));
