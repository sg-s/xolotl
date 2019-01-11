
%% demo_approx
% In this example, we show how the approx_channels
% property can be used to speed up simulations
% Note that the approximate solution is different
% from the solution that doesn't use Look-up-Tables

close all

x = xolotl.examples.BurstingNeuron('prinz',14.96);

x.t_end = 1e4;
x.integrate;


x.closed_loop = false;
x.sim_dt = .05;
x.dt = .1;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
x.t_end = 100e3; x.sim_dt = .05; tic; V = x.integrate; t =  toc;
x.t_end = 5e3; V = x.integrate;
disp(['Exact speed = ' oval(100/t) 'X'])
plot(V,'k')

x.approx_channels = 1;
x.t_end = 100e3; x.sim_dt = .05; tic; V = x.integrate; t =  toc;
x.t_end = 5e3; V = x.integrate;
disp(['Approximate speed = ' oval(100/t) 'X'])
plot(V,'r')
legend({'Exact','Approximate'})
