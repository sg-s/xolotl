

close all


x = xolotl.examples.BurstingNeuron('prinz',14.96);

x.t_end = 1e4;
x.integrate;
x.t_end = 5e3;



x.closed_loop = false;
x.sim_dt = .1;
x.dt = .1;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
V = x.integrate;
plot(V,'k')

x.solver_order = 4;
[V, Ca] = x.integrate;
plot(V,'r')
