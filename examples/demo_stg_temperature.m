% test script for matlab wrapper

% this sets up the STG network
% as in Fig 2e of this paper:
% Prinz ... Marder Nat Neuro 2004
% http://www.nature.com/neuro/journal/v7/n12/abs/nn1352.html



x = xolotl.examples.neurons.BurstingNeuron('prefix','prinz-temperature','CalciumMech','buchholtz-temperature');

x.set('*Q',2 + rand(19,1))

x.sim_dt = .1;
x.dt = .1;
x.t_end = 5e3;

x.closed_loop = false;

V = x.integrate;
time = (1:length(V))*1e-3*x.dt;

x.temperature = 22;
V2 = x.integrate;

figure('outerposition',[0 0 1000 500],'PaperUnits','points','PaperSize',[1000 500]); hold on

plot(time,V,'k')
plot(time,V2,'r')
xlabel('Time (s)')
ylabel('V_{m} (mV)')
legend({'11C','22C'})
set(gca,'XLim',[0 6],'YLim',[-80 50])
figlib.pretty('PlotLineWidth',1,'LineWidth',1)
drawnow

