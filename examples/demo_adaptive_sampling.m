% In this demo, we show how we can use adaptive sampling
% to efficiently map parameter space

%% Create a single-compartment neuron model
x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');
x.dt = .1;
x.sim_dt = .1;
x.t_end = 10e3;

% choose which parameters to vary
parameters_to_vary = {'*.HCurrent.gbar','*.CaS.gbar'};

%% Create the adaptive sampler object
a = adaptive('MaxIter',20);

% create the sampling function
% here, we are sampling the burst period
% and varying the H current and CaS maximal conductances
a.SampleFcn = @(params) xtools.sampleMetrics(x,[],parameters_to_vary,params);

% set up lower and upper bounds for params
a.Lower = [0 0];
a.Upper = [20 100];

a.UseParallel = true;

%% Generate a figure
figure('outerposition',[300 300 601 600],'PaperUnits','points','PaperSize',[601 600]); hold on
a.PlotHere = gca;

figlib.pretty('LineWidth',1)

xlabel('g_H (uS/mm^2)')
ylabel('g_{CaS} (uS/mm^2)')
c = colorbar;
c.Label.String = 'burst period (ms)';

%% Perform the sampling and plot onto the created figure
a.sample
