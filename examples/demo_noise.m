

x = xolotl.examples.BurstingNeuron;

x.AB.add('goldwyn/CurrentNoise2','noise_amplitude',2);

x.sim_dt = .1;
x.dt = .1;

x.integrate;


x.t_end = 2e3;

x.snapshot('zero');

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
V = x.integrate;
time = (1:length(V))*x.dt*1e-3;
plot(time,V,'k')
drawnow

x.reset('zero')
V = x.integrate;
plot(time,V,'r')
xlabel('Time (s)')
drawnow

x.reset('zero')
x.set('*noise_amplitude',0)
x.AB.HCurrent.add('SubunitNoise','noise_amplitude',.002);

V = x.integrate;
plot(time,V,'b')
drawnow
