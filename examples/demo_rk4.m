

close all


x = xolotl.examples.neurons.BurstingNeuron('prefix','prinz');

x.t_end = 1e4;
x.integrate;
x.t_end = 5e3;



x.closed_loop = false;
x.sim_dt = .1;
x.dt = .1;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
V = x.integrate;
time = (1:length(V))*1e-3*x.dt;
plot(time,V,'k')

x.solver_order = 4;
[V, Ca] = x.integrate;
plot(time,V,'r')

set(gca,'YLim',[-80 70])
ylabel('V_m (mV)')
xlabel('Time (s)')

legend({'Exponential Euler','Runge-Kutta 4'})

figlib.pretty('PlotLineWidth',1,'LineWidth',1)