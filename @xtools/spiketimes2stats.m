% _       _   _
% __  _____ | | ___ | |_| |
% \ \/ / _ \| |/ _ \| __| |
% >  < (_) | | (_) | |_| |
% /_/\_\___/|_|\___/ \__|_|
%
% static method of xtools to help analyze experimental data
% here, we only assume that we know spiketimes, and use those
% to measure burst metrics for each neuron, and the phase differences
% between them
%
% here is a description of what this function returns:
% neuron_metrics
% ==============
% (1) time period of oscillation
% (2) CV of time period of oscillation
% (3) duty cycle
% (4) CV of duty cycle
% (5) #spikes/burst
% (6) CV of (5)
% (7) mean firing rate
% (8) CV of ISI
%
% phase_differences
% =================
% (1) mean phase difference
% (2) CV of phase difference
%
% Srinivas Gorur-Shandilya

function [neuron_metrics, phase_differences] = spiketimes2stats(varargin)

% figure out how many neurons we are working with
N = length(varargin);

% make sure all the inputs are the same size
if N > 1
	for i = 2:N
		assert(size(varargin{i},1) == size(varargin{1},1),'All inputs should be the same size')
		assert(size(varargin{i},2) == size(varargin{1},2),'All inputs should be the same size')
	end
end

% if the inputs are matrices, call this function recursively
if size(varargin{1},2) > 1
	neuron_metrics = cell(N,1);
	phase_differences = cell(N,1);
	for i = 1:N
		neuron_metrics{i} = NaN(8,size(varargin{1},2));
	end
	for i = 1:N
		phase_differences{i} = NaN(2,size(varargin{1},2));
	end


	for i = 1:size(varargin{1},2)
		these_spiketimes = cell(N,1);
		for j = 1:N
			these_spiketimes{j} = varargin{j}(:,i);
		end
		[these_neuron_metrics, these_phase_differences] = xtools.spiketimes2stats(these_spiketimes{:});
		for j = 1:N
			neuron_metrics{j}(:,i) = these_neuron_metrics{j};
			phase_differences{j}(:,i) = these_phase_differences{j};
		end
	end

	return
end

% make the placeholders
neuron_metrics = cell(N,1);
phase_differences = cell(N,1);
for i = 1:N
	neuron_metrics{i} = NaN(8,1);
end
for i = 1:N
	phase_differences{i} = NaN(2,1);
end


% convert spiketimes into pseudo timeseries using a kernel
% to smooth over spikes

for i = 1:N

	if any(varargin{i})
		isi = diff(find(varargin{i}));
		if length(isi) > 11
			misi = percentile(isi,.1);
		else
			misi = min(isi);
		end
		if 	max(isi)/misi > 10
			% bursting
			tau = mean(isi);
		else
			% probably spiking
			tau = 1;
		end

		p = struct('tau',tau,'n',1,'A',1);
		K = filter_gamma(1:2e3,p);
		X{i} = fastFilter(K,1,full(varargin{i}));
	else
		% no spikes
		X{i} = full(varargin{i});
	end
end



for i = 1:N

	if ~any(X{i})
		neuron_metrics{i}(1) = -1; % time period not defined
		neuron_metrics{i}(2) = -1; % time period not defined

		neuron_metrics{i}(3) = -1; % duty cycle not defined
		neuron_metrics{i}(4) = -1; % time period not defined

		neuron_metrics{i}(5) =  0; % #spikes/burst
		neuron_metrics{i}(6) = 0;

		neuron_metrics{i}(7) =  0; % firing rate
		neuron_metrics{i}(8) = 0;  % CV of ISI

		if i > 1
			phase_differences{i}(1:2) = -1; % phase differences not defined
		end

		continue
	end

	% find all the peaks using a robust search for maxima
	[ons, offs] = computeOnsOffs(X{i} > mean(nonzeros(X{i})));

	% find peaks within this
	spike_peaks{i} = NaN*ons;
	for j = 1:length(ons)
		[~,spike_peaks{i}(j)] = max(X{i}(ons(j):offs(j)));

	end
	spike_peaks{i} = spike_peaks{i} + ons - 1;

	% for each peak, find the location of the minimum preceding it
	spike_mins = NaN*ons;
	burst_durations = NaN*ons;
	n_spike_per_burst = NaN*ons;

	for j = 2:length(ons)
		[~,idx] = min(X{i}(ons(j-1):ons(j)));
		idx = idx(end);
		spike_mins(j) = ons(j-1) + idx;

		bd = find(varargin{i}(spike_mins(j):spike_peaks{i}(j)),1,'last');
		if isempty(bd)
			% only 1 spike in this "burst", so
			% count it as 0
			bd = 0;
		end

		burst_durations(j) = bd;

		n_spike_per_burst(j) = full(sum(varargin{i}(spike_mins(j):spike_peaks{i}(j))));
	end

	% burst periods + variability
	neuron_metrics{i}(1) = mean(diff(spike_peaks{i}));
	neuron_metrics{i}(2) = std(diff(spike_peaks{i}))/neuron_metrics{i}(1);

	% duty cycle + variability
	duty_cycle = burst_durations(2:end)./diff(spike_peaks{i});
	neuron_metrics{i}(3) = mean(duty_cycle);
	neuron_metrics{i}(4) = std(duty_cycle)/neuron_metrics{i}(3);

	% n_spike_per_burst + variability
	neuron_metrics{i}(5) = mean(n_spike_per_burst(2:end));
	neuron_metrics{i}(6) = std(n_spike_per_burst(2:end))/neuron_metrics{i}(5);

	% mean firing rate + CV of ISI
	spiketimes = find(varargin{i});
	neuron_metrics{i}(7) = length(spiketimes)/length(X{i});
	if length(spiketimes) > 1
		neuron_metrics{i}(8) = cv(diff(spiketimes));
	else
		neuron_metrics{i}(8) = 0;
	end

	if i > 1

		% now compute the phase differences

		phase_diff = NaN*spike_peaks{1};
		for j = 1:length(spike_peaks{1}) - 1
			% find spike peaks in this neuron in the interval between
			% jth and (j+1)th spike_peak in the 1st neuron
			this_neuron_peaks = spike_peaks{i}(spike_peaks{1}(j) < spike_peaks{i} & spike_peaks{1}(j+1) > spike_peaks{i});
			if isempty(this_neuron_peaks)
				continue
			end
			this_neuron_peaks = this_neuron_peaks(1);
			phase_diff(j) =  (this_neuron_peaks - spike_peaks{1}(j))/(spike_peaks{1}(j+1) - spike_peaks{1}(j));
		end


		phase_differences{i}(1) = nanmean(phase_diff);
		phase_differences{i}(2) = nanstd(phase_diff)/phase_differences{i}(1);

	end

end
