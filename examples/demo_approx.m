
%% demo_approx
% In this example, we show how the approx_channels
% property can be used to speed up simulations
% Note that the approximate solution is different
% from the solution that doesn't use Look-up-Tables

close all

x = xolotl.examples.neurons.BurstingNeuron();

x.t_end = 1e4;
x.integrate;


x.closed_loop = false;
x.sim_dt = .05;
x.dt = .1;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
x.approx_channels = 0;
x.t_end = 100e3; x.sim_dt = .05; tic; V = x.integrate; t =  toc;
x.t_end = 5e3; V = x.integrate;
time = (1:length(V))*x.dt*1e-3;
disp(['Exact speed = ' strlib.oval(100/t) 'X'])
plot(time,V,'k')

x.approx_channels = 1;
x.t_end = 100e3; x.sim_dt = .05; tic; V = x.integrate; t =  toc;
x.t_end = 5e3; V = x.integrate;
disp(['Approximate speed = ' strlib.oval(100/t) 'X'])
plot(time,V,'r')
legend({'Exact','Approximate'})

set(gca,'YLim',[-80 50])
ylabel('V_m (mV)')
xlabel('Time (s)')

figlib.pretty('PlotLineWidth',1.5,'LineWidth',1.5)