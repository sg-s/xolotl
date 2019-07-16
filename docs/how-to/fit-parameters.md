This document details how to optimize parameters of a xolotl object to fit some constraints using the `xfit` class.

Parameter optimization is the process of changing some parameters of a model so that the model meets some criteria set by the modeler, as closely as possible.
It consists of designing a cost (or objective) function that outputs a positive real-valued result called the cost.
The trick is to design a cost function so that when the model does what you want, the cost is very low,
and when the model has some aberrant behavior, the cost is very high.
We simulate the model and compute the cost for some set of parameters,
then algorithmically test new parameters
until we find a parameter set that best suits the cost function
(by minimizing the cost).

In this document, we will consider a bursting neuron
and try to change its maximal conductances to change its burst period to a desired value.

### Instantiating the xolotl object and the xfit object

First, let us construct our model.
This is an eight-conductance, single-compartment model.
Using conductance dynamics from Liu *et al.* 1998.

```matlab
x = xolotl.examples.BurstingNeuron('prefix', 'liu');
```

We will then instantiate the xfit object
and specify the particle swarm algorithm.

```matlab
p = xfit('particleswarm');
```

### Designing a cost function

We will design a very simple cost function that computes the burst period and number of spikes per burst.

```matlab
function [C, burst_period, n_spikes_per_burst] = cost_function(x, ~, ~)

  % reset the model
  x.reset;
  % set up the simulation time, and time step
  x.t_end = 10e3;
  x.sim_dt = 0.1;
  x.dt = 0.1;
  % use approximate channel dynamics for speedy computation
  x.approx_channels = 1;
  % turn on closed_loop integration
  x.closed_loop = true;

  % integrate the model once to get to steady-state
  x.integrate;
  % integrate from steady-state and store the output
  V = x.integrate;

  % compute the burst metrics
  metrics = xtools.V2metrics(V, 'sampling_rate', 10);
  burst_period = metrics.burst_period;
  n_spikes_per_burst = metrics.n_spikes_per_burst_mean;

  % if the burst period is between 930 and 1050 ms, cost is zero
  % otherwise, it's quadratic (bigger further away from bin)
  C = xfit.binCost([930, 1050], burst_period);

  % if the mean number of spikes per burst is between 7 and 10, cost is zero
  % otherwise, it's quadratic (bigger way from bin)
  C = C + xfit.binCost([7, 10], n_spikes_per_burst);

  % if something goes terribly wrong, return a super high cost
  if isnan(C)
    C = 1e3;
  end

end % function
```

Now, we add the cost function to the xfit object.

```matlab
p.sim_func = @cost_function;
```

### Setting up parameter optimization

We will optimize over all maximal conductances.

```matlab
p.parameter_names = x.find('*gbar')
```

and we define upper bound and lower bound values for our parameters.
We know these values by considering the biology.

```matlab
% lower bound values
p.lb = [100 0 0 0 0 500 500];
% upper bound values
p.ub = [1e3 100 100 10 500 2000 2000];
```

### Performing the simulation

We display the results before simulation.

```matlab
% display the results before optimization
figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on

subplot(2,1,1); hold on

x.t_end = 10e3;
V = x.integrate;
time = (1:length(V))*1e-3*x.dt;
plot(time,V,'k')
title('Before optimization')
```

and begin the optimization procedure

```matlab
p.fit;
```

The results are saved in the `seed` property of the xfit object,
so we will update our xolotl object with those maximal conductances.

```matlab
x.set('*gbar', p.seed);
```

Finally, we can finish our plot, and show voltage waveform produced by the optimized model.

```matlab
% visualize the results of the optimization
subplot(2,1,2); hold on

x.t_end = 10e3;
V = x.integrate;
time = (1:length(V))*1e-3*x.dt;
plot(time,V,'r')
title('After optimization')

figlib.pretty('LineWidth', 1, 'PlotLineWidth', 1, 'PlotBuffer', 0.2)
```

### Next steps

We have successfully performed one round of optimization.
We could consider performing the optimization multiple times,
or varying the initial parameters to try to get a better sense
of the parameter space as a whole.
It is also possible to run xfit optimizations in a loop
to try to find many parameter sets that satisfy the constraints.
It is often true in neuronal modeling that many parameter sets, even ones that are very distant from each other, produce similar neuronal activity.
