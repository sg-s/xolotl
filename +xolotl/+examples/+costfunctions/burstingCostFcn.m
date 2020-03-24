

% this example simulation function simulates a model
% and computes the burst period and mean spikes per burst
% then, it returns a cost
% if the burst period is within [950, 1050] ms, then that part of the cost is zero
% if the mean spikes per burst is within [7, 10] then that part of the cost is zero
% otherwise, the cost is the quadratic difference


function [C, V] = burstingCostFcn(x,~)

% x is a xolotl object
x.reset;
x.t_end = 10e3;
x.approx_channels = 1;
x.sim_dt = .1;
x.dt = .1;
x.closed_loop = true;

x.integrate;
V = x.integrate;

% measure behaviour
metrics = xtools.V2metrics(V,'sampling_rate',10);


% accumulate errors
C = xtools.binCost([950 1050],metrics.burst_period);
C = C + xtools.binCost([.1 .3],metrics.duty_cycle_mean);
C = C + xtools.binCost([7 10],metrics.n_spikes_per_burst_mean);

% safety -- if something goes wrong, return a large cost
if isnan(C)
	C = 1e3;
end
