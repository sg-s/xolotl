In this tutorial, we will walk through the process of creating a parallelized parameter sweep in xolotl.

We will set up a simple model,
and simulate it many times in parallel for many parameter values.

!!! Note
    In most cases, you don't need to use `xgrid`. If you want to run all your code in parallel on a single computer, you can use MATLAB's native parallel support to do this. Run the `demo_parallel` example to see how this works. 

### A high-level view of xgrid

xgrid is an extension to the xolotl framework that allows for parallelized computations.
This means that multiple processors on a computer (or computers)
can contribute to simulating xolotl models.
If you are working on a six-core computer with 12 workers,
it is possible to get up to a 12x increase in simulation speed
by running your simulations in parallel.

xgrid can be used to simulate models on multiple computers remotely,
but here we will focus on doing so using only a local computer.

### Creating the model

We will start by creating a xolotl model.
Here, we will use a bursting neuron model with conductances from Liu *et al.* 1998.
This model is built-in to xolotl as an example, so we will instantiate it.

```matlab
x = xolotl.examples.neurons.BurstingNeuron('prefix', 'liu');
x.dt = 0.1; % ms
x.sim_dt = 0.1; % ms
x.t_end = 10e3; % ms
```

We have created an eight-conductance single-compartment model
and set the time resolution and total simulation time.

### Designing a parameter sweep

We are interested in considering how the slow calcium and a-type potassium current affect the burst period and number of spikes per burst.

```matlab
% identify which parameters we want to consider
parameters_to_vary = {'*.CaS.gbar', '*.ACurrent.gbar'};

% create vectors for the parameter values
g_CaS_space = linspace(0,100,25);
g_A_space = linspace(100,300,25);

% create a 2 x N matrix of parameter values
% where N is the number of simulations
all_params = NaN(2,length(g_CaS_space)*length(g_A_space));
c = 1;
for i = 1:length(g_CaS_space)
	for j = 1:length(g_A_space)
		all_params(1,c) = g_CaS_space(i);
		all_params(2,c) = g_A_space(j);
		c = c + 1;
	end
end
```

We now have a 2 x N matrix of parameter values corresponding to the
cell array of xolotl parameter names.

### Creating the xgrid object

Let's create the xgrid object, telling it that we want to use the local machine to run simulations. 

```matlab
p = xgrid('local');
```

We will cleanup any old auxiliary files that are laying around,
and set the number of batches to 3.
This way, the simulations will be performed in three chunks.
We will then set up the xolotl object in xgrid,
and finally, create the jobs for xgrid to run in parallel.

```matlab
p.cleanup
p.n_batches = 3;
p.x = x;
p.batchify(all_params, parameters_to_vary);
```

### Configuring the simulation

Let's suppose that we are interested in considering how the CaS and ACurrent conductances affect the burst period and number of spikes per burst.
Let's also keep track of the simulation time.

We will need a simulation function with the following signature:

```matlab
function [burst_period, n_spikes_per_burst, sim_time] = xgrid_test_func(x,~,~)
```

The body of the function should integrate the model
and use some analysis functions to compute the burst period and number of spikes per burst.

```matlab
p.sim_func = @xgrid_test_func;
```


### Performing the simulation

We are ready to perform the simulation.
To do this, we use the `simulate` function,
and then wait until all workers have finished.

```matlab
p.simulate;
wait(p.workers);
```

### Gathering the data

We have finished the simulation, but the data have not been collected.
To do this, we will use the gather function,
which returns the outputs of `p.sim_func` for all parameters as a cell array.

```matlab
[all_data,all_params,all_param_idx] = p.gather;
burst_periods = all_data{1};
n_spikes_per_burst = all_data{2};
spiketimes = all_data{3};
```

### Visualizing the data

We would like to reconstruct the simulation results into a matrix
so that we can make easy-to-interpret figures.
To do this, we will construct matrices of the burst period and number of spikes per burst
where each element is indexed by the slow calcium conductance and A-type current that gave rise to that simulation result.

```matlab
% assemble the data into a matrix for display
BP_matrix = NaN(length(g_CaS_space),length(g_A_space));
NS_matrix = NaN(length(g_CaS_space),length(g_A_space));

% populate the matrices using linear indexing
for i = 1:length(all_params)
	xx = find(all_params(1,i) == g_CaS_space);
	y = find(all_params(2,i) == g_A_space);
	BP_matrix(xx,y) = burst_periods(i);
	NS_matrix(xx,y) = n_spikes_per_burst(i);
end

% remove any erroneous results
BP_matrix(BP_matrix<0) = NaN;
NS_matrix(NS_matrix<0) = 0;
```

We can now plot the results.

```matlab
figure('outerposition',[0 0 1100 500],'PaperUnits','points','PaperSize',[1100 500]); hold on
subplot(1,2,1)
h = heatmap(g_A_space,g_CaS_space,BP_matrix);
h.Colormap = parula;
h.MissingDataColor = [1 1 1];
ylabel('g_CaS')
xlabel('g_A')
title('Burst period (ms)')

subplot(1,2,2)
h = heatmap(g_A_space,g_CaS_space,NS_matrix);
h.Colormap = parula;
h.MissingDataColor = [1 1 1];
ylabel('g_CaS')
xlabel('g_A')
title('#spikes/burst')

figlib.pretty();
```