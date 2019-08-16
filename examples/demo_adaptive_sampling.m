% In this demo, we show how we can use adaptive sampling
% to efficiently map parameter space

x = xolotl.examples.BurstingNeuron('prefix','liu');
x.dt = .1;
x.sim_dt = .1;
x.t_end = 10e3;

parameters_to_vary = {'*.HCurrent.gbar','*.CaS.gbar'};

a = adaptive('MaxIter',20);
a.SampleFcn = @(params) xtools.sampleMetrics(x,[],parameters_to_vary,params);

a.Lower = [0 0];
a.Upper = [20 100];

a.UseParallel = true;

figure('outerposition',[300 300 601 600],'PaperUnits','points','PaperSize',[601 600]); hold on
a.PlotHere = gca;

figlib.pretty('LineWidth',1)

xlabel('g_H (uS/mm^2)')
ylabel('g_{CaS} (uS/mm^2)')

a.sample