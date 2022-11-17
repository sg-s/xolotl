% in this example script, we take a neuron
% and optimize its maximal conductances so that it has a
% desired period

if license('test', 'global_optimization_toolbox') == 0
	% can't do anything, we don't have the toolbox
	return
end

% first, we create our xolotl object
x = xolotl.examples.neurons.BurstingNeuron('prefix','prinz');

% try to download a binary
% so that people without compilers can use this too
try
	x.snapshot('base')
	x.integrate;
	x.reset('base')
catch
	x.download;
end




x.set('*gbar',veclib.shuffle((x.get('*gbar'))))

% we instantiate the xfit object
% and specify the particle swarm engine
p = xfit('particleswarm');
p.options.UseParallel = true;

% we assign a cost function
p.SimFcn = @xolotl.examples.costfunctions.burstingCostFcn;


% we assign the xolotl object
p.x = x;

% we optimzie over all maximal conductances
p.FitParameters = x.find('*gbar');
% lower bound values
p.lb = [100 0 0 0 0 500 0 500];
% upper bound values
p.ub = [1e3 100 100 10 500 2000 1 2000];

% display the results before optimization
figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on

subplot(2,1,1); hold on
set(gca,'XLim',[0 10],'YLim',[-80 50])
x.t_end = 10e3;
V = x.integrate;
time = (1:length(V))*1e-3*x.dt;
plot(time,V,'k')
title('Before optimization')

subplot(2,1,2); hold on
set(gca,'XLim',[0 10],'YLim',[-80 50])


% perform the optimization procedure
p.fit;

% the best fit is stored in p.seed
% set the existing xolotl object to match
x.set('*gbar',p.seed)


% visualize the results of the optimization

x.t_end = 10e3;
V = x.integrate;
time = (1:length(V))*1e-3*x.dt;
plot(time,V,'r')
title('After optimization')

figlib.pretty('LineWidth', 1, 'PlotLineWidth', 1, 'PlotBuffer', 0)
