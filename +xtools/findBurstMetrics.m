%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### findBurstMetrics





**Syntax**

```matlab
xtools.findBurstMetrics(V, Ca)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca, Ca_peak_similarity)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca, Ca_peak_similarity, burst_duration_variability)
[burst_metrics, spike_times, Ca_peaks, Ca_troughs] = xtools.findBurstMetrics(V, Ca, Ca_peak_similarity, burst_duration_variability, on_off_thresh)
```

**Description**

Computes burst metrics, spike times, and peaks and troughs of calcium wave.

**Arguments**

`V` is an n x 1 vector describing the membrane potential of a compartment over time.

`Ca` is an n x 1 vector describing the intracellular calcium concentration of the
same compartment over time.

| Optional Positional Argument | Default Value |
| --------  | ------ |
| `Ca_peak_similarity` | 0.3
| `burst_duration_variability` | 0.1 |
| `on_off_thresh` | 0 |

`Ca_peak_similarity` sets the maximum acceptable coefficient of variation (CV) between
the intracellular calcium peaks. This function will exit with an error code if this limit is
exceeded. To ignore this limit, set it to `Inf`.

`burst_duration_variability` sets the maximum acceptable CV between the burst periods.
This function will exit with an error code if this limit is exceeded. To ignore this
limit, set it to `Inf`.

`on_off_thresh` determines the horizontal crossing line (in units of mV) at which
spikes should be counted. For example, if the `on_off_thresh` is set to 10 mV, then
only when the membrane potential crosses 10 mV will a spike be counted.

**Outputs**

With no outputs, the burst period, mean number of spikes per burst, and duty cycle
are printed to the command window.

`burst-metrics` is a 10x1 vector which contains the

1.  burst period (in units of dt)
2.  mean number of spikes per burst (unitless)
3.  time of first spike within a burst relative to  the burst's Ca peak (in units of dt)
4.  time of last spike within a burst relative to  the burst's Ca peak (in units of dt)
5.  mean of calcium peaks ($\mu M$)
6.  mean of Calcium troughs ($\mu M$)
7.  variability of Calcium peaks (coefficient of variation)
8.  variability of burst periods (coefficient of variation)
9.  duty cycle (unitless)
10. error code

The error code is a digit between 0 and 4 that describes the success of the function.

0. 	No error
1.	Fewer than 5 Calcium peaks
2. 	Calcium peaks not similar enough
3. 	Burst periods too variable
4. 	No spikes

`spike_times` is an n x 1 vector containing the times (in units of dt) of each spike
in the trace defined by V.

`Ca_peaks` is an n x 1 vector containing the times (in units of dt) of each peak
of the calcium wave.

`Ca_troughs` is an n x 1 vector containing the times (in units of dt) of each trough
of the calcium wave.

!!! info "Dependencies"
    This function requires the Signal Processing Toolbox for MATLAB.

%}

function [burst_metrics, spike_times, Ca_peaks, Ca_troughs] = findBurstMetrics(V,Ca,Ca_peak_similarity, burst_duration_variability,on_off_thresh)

assert(isvector(V),'Voltage trace has to be a vector')
assert(isvector(Ca),'Calcium trace has to be a vector')

if nargin < 3
	Ca_peak_similarity = .3;
end
if nargin < 4
	burst_duration_variability = .1;
end
if nargin < 5
	on_off_thresh = 0;
end

burst_metrics = -ones(10,1);
Ca_peaks = [];
spike_times = [];
Ca_troughs = [];

Ca_prom = std(Ca);
[peak_Ca,burst_peak_loc] = findpeaks(Ca,'MinPeakProminence',Ca_prom);

Ca_peaks = burst_peak_loc;

% find spikes
spike_times = nonnans(xtools.findNSpikeTimes(V,1000,on_off_thresh));

if length(spike_times) == 0
	burst_metrics(10) = 4;
	return
end

% there should be at least 5
if length(peak_Ca) < 5
	burst_metrics(10) = 1;
	return
end

% check for similarity of peak heights
cv_peak_Ca = std(peak_Ca)/(mean(peak_Ca));
if  cv_peak_Ca > Ca_peak_similarity
	burst_metrics(10) = 2;
	burst_metrics(7) = cv_peak_Ca;
	return
end

burst_periods = diff(burst_peak_loc);
cv_burst_periods = std(burst_periods)/mean(burst_periods);
if cv_burst_periods > burst_duration_variability
	burst_metrics(10) = 3;
	burst_metrics(8) = cv_burst_periods;
	return
end

mean_burst_period = mean(burst_periods);


n_spikes = NaN*burst_peak_loc;
first_spike_loc = NaN*burst_peak_loc;
last_spike_loc = NaN*burst_peak_loc;
duty_cycle = NaN*burst_peak_loc;
cal_min = NaN*burst_peak_loc;


if nargout == 0

	figure, hold on
	time = 1:length(V);
	plot(time,V,'k')
end


% for each burst, look around that peak and count spikes
% we skip the first and last Calcium maxima
% so that we are guaranteed to have something on either side
for i = 2:length(burst_peak_loc-1)


	% we consider spikes to belong the current calcium peak if they
	% occur after a and before z
	% where a is the half-way point b/w the previous Ca max
	% and the preceding Ca minimum, and where
	% z is the half-way point b/w the current Ca max and the next Ca minimum

	% find the preceding calcium minimum
	[~,idx] = min(Ca(burst_peak_loc(i-1):burst_peak_loc(i)));
	prev_ca_min = idx + burst_peak_loc(i-1);
	cal_min(i) = prev_ca_min;
	a = floor((burst_peak_loc(i-1) + prev_ca_min)/2);


	% find calcium minimum after current peak
	if i == length(burst_peak_loc)
		[~,idx] = min(Ca(burst_peak_loc(i):end));
	else
		[~,idx] = min(Ca(burst_peak_loc(i):burst_peak_loc(i+1)));
	end
	next_ca_min = idx + burst_peak_loc(i);
	z = floor((burst_peak_loc(i) + next_ca_min)/2);



	% find spikes in this interval
	spikes_in_this_burst = spike_times(spike_times < z & spike_times > a);

	if nargout == 0

		scatter(time(a),V(a),'ko')
		scatter(time(z),V(z),'gx')
		scatter(time(spikes_in_this_burst),V(spikes_in_this_burst),'ro')

		y = [max(V) + 10, max(V) + 10];
		x = [spikes_in_this_burst(1) spikes_in_this_burst(end)];
		plot(x,y,'r','LineWidth',5)

	end


	n_spikes(i) = length(spikes_in_this_burst);

	if n_spikes(i) > 0

		first_spike_loc(i) = spikes_in_this_burst(1) - burst_peak_loc(i);
		last_spike_loc(i) =  spikes_in_this_burst(end) - burst_peak_loc(i);

		duty_cycle(i) = spikes_in_this_burst(end) - spikes_in_this_burst(1);

	end


end

Ca_troughs = cal_min;

burst_metrics(1) = mean_burst_period;
burst_metrics(2) = mean(n_spikes(2:end-1));
burst_metrics(3) = mean(first_spike_loc(2:end-1));
burst_metrics(4) = mean(last_spike_loc(2:end-1));
burst_metrics(5) = mean(peak_Ca(2:end-1));
burst_metrics(6) = mean(cal_min(2:end-1));


burst_metrics(9) = mean(duty_cycle(2:end-1))/mean_burst_period;

burst_metrics(10) = 0;

if nargout == 0
	fprintf('Burst metrics:\n')
	fprintf('--------------\n')
	fprintf(['Burst period:       '  oval(burst_metrics(1)) '\n'])
	fprintf(['# spikes/burst:     '  oval(burst_metrics(2)) '\n'])
	fprintf(['Duty Cycle:         '  oval(burst_metrics(9),3) '\n'])
end
