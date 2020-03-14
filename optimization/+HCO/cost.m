function COST = cost(x,data)


% copy over parameters to the other neuron
x.set(data.other_params,x.get(data.parameter_names))

gmax = x.Cell1.GradedCell2.gmax;

COST = 0;

% parameters
A1 = 300; % synapse off firing rate
A2 = 100; % regular spiking cost
B = 10; % burst period
C = 100; % duty cycle
D = 100; % nspikes/burst
E = 100; % ratio of firing rates of two cells
F = 200; % phase

x.reset;

Ca_burster = NaN;
Ca_CB0 = NaN;
Ca_CB1 = NaN;

% don't change these!

x.dt = .1;
x.sim_dt = .1;

% turn synapse off. now, we want both neurons to be
% tonically spiking or silent. since the two neurons are identical
% it is sufficient to check only one

% x.reset;
% x.set('*gmax',0)
% x.closed_loop = false;

% x.t_end = 20e3;
% V = x.integrate;
% V(1:1e4,:) = [];

% m = xtools.V2metrics(V(:,1),'sampling_rate',10,'spike_threshold',-20);

% COST = COST + A1*xtools.binCost([0, 20], m.firing_rate);


% COST = COST + A2*xtools.binCost([0, 1e-2],m.isi_std/m.isi_mean);



% now configure the synapse
x.set('*gmax',gmax);
x.reset;
x.Cell1.V = -61; % a little push to get it out of the synchronous state

x.t_end = 40e3;
x.closed_loop = true;
x.integrate;
V = x.integrate;

m(2) = xtools.V2metrics(V(:,2),'sampling_rate',10,'spike_threshold',-20);
m(1) = xtools.V2metrics(V(:,1),'sampling_rate',10,'spike_threshold',-20);

COST = COST + B*xtools.binCost([3e3 20e3],m(1).burst_period) + B*xtools.binCost([500 2e3],m(2).burst_period);


% % duty cycle
% COST = COST + C*xtools.binCost([.3 .6],m(1).duty_cycle_mean) + C*xtools.binCost([.3 .6],m(2).duty_cycle_mean);


% % nspikes/burst
% COST = COST + D*xtools.binCost([10 40],m(1).n_spikes_per_burst_mean);
% COST = COST + D*xtools.binCost([10 40],m(2).n_spikes_per_burst_mean);

return

% phase

% find the burst starts and ends in the master cell 
try
	spike_times = xtools.findNSpikeTimes(V(:,1),xtools.findNSpikes(V(:,1)));
	spike_times2 = xtools.findNSpikeTimes(V(:,2),xtools.findNSpikes(V(:,2)));
	ibi = 3000;
	burst_ends = spike_times(find(diff(spike_times) > ibi));

	burst_starts = find(diff(spike_times) > ibi) + 1;
	burst_starts(burst_starts>length(spike_times)) = [];
	burst_starts = spike_times(burst_starts);

	if burst_ends(1) < burst_starts(1)
		burst_ends(1) = [];
	end

	n_ok_spikes = 0;
	n_wrong_spikes = 0;

	M = min([length(burst_starts) length(burst_ends)]);

	for i = 2:M-1
		n_wrong_spikes = n_wrong_spikes + sum(spike_times2 > burst_starts(i) &  spike_times2 < burst_ends(i));

		n_ok_spikes = n_ok_spikes + sum(spike_times2 > burst_ends(i-1) &  spike_times2 < burst_starts(i));

	end


	COST = COST + F*xtools.binCost([0, .1],n_wrong_spikes/(n_wrong_spikes + n_ok_spikes)) ...
   +F*xtools.binCost([.9, 1],n_ok_spikes/(n_wrong_spikes + n_ok_spikes));
catch
	COST = COST + 2*F;
end


