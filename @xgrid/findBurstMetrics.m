%                          _
%                         | |
%     _ __  ___ _   _  ___| |__   ___  _ __   ___  _ __ ___  _ __
%    | '_ \/ __| | | |/ __| '_ \ / _ \| '_ \ / _ \| '_ ` _ \| '_ \
%    | |_) \__ \ |_| | (__| | | | (_) | |_) | (_) | | | | | | |_) |
%    | .__/|___/\__, |\___|_| |_|\___/| .__/ \___/|_| |_| |_| .__/
%    | |         __/ |                | |                   | |
%    |_|        |___/                 |_|                   |_|
%
% Explanation of outputs
% ======================
%
% findBurstMetrics finds the following things,
% which are returned in a 10-element vector
% (1)  burst period (in units of dt)
% (2)  # of spikes / burst
% (3)  time of first spike relative to Ca peak (in units of dt)
% (4)  time of last spike relative to Ca peak (in units of dt)
% (5)  mean height of calcium peak (uM)
% (6)  mean minimum of Calcium minimum (uM)
% (7)  variability of Calcium peaks (CV)
% (8)  variability of burst periods (CV)
% (9)  duty cycle
% (10) error code (see below for details)
%
% in addition, it returns the time of every spike,
% the times of the calcium peaks, and the times
% of the calcium minimums in additional outputs
%
% minimal usage:
% ==============
%
% [burst_metrics, spike_times, Ca_peaks, Ca_mins] = findBurstMetrics(V,Ca)
%
% note that while burst_metrics is always 10 elements long,
% the other three outputs can have variable lengths
%
% Explanation of error codes
% ==========================
%
% 0 	No error
% 1		Fewer than 5 Calcium peaks
% 2 	Calcium peaks not similar enough
% 3 	Burst periods too variable
% 4 	No spikes

function [burst_metrics, spike_times, Ca_peaks, Ca_mins] = findBurstMetrics(V,Ca,Ca_peak_similarity, burst_duration_variability,on_off_thresh)

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
Ca_mins = [];

Ca_prom = std(Ca);
[peak_Ca,burst_peak_loc] = findpeaks(Ca,'MinPeakProminence',Ca_prom);

Ca_peaks = burst_peak_loc;

% find spikes
spike_times = veclib.nonnans(xtools.findNSpikeTimes(V,1000,on_off_thresh));

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

Ca_mins = cal_min;

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
	fprintf(['Burst period:       '  strlib.oval(burst_metrics(1)) '\n'])
	fprintf(['# spikes/burst:     '  strlib.oval(burst_metrics(2)) '\n'])
	fprintf(['Duty Cycle:         '  strlib.oval(burst_metrics(9),3) '\n'])
end
