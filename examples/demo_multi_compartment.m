% makes a realistic cable with some options


x = xolotl.examples.BurstingCable();
x.dt = .1;
x.sim_dt = .1;


V = x.integrate;
time = (1:length(V))*x.dt*1e-3;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
subplot(2,1,1); hold on
plot(time,V(:,1),'k')
xlabel('Time (s)')
ylabel('Voltage in soma (mV)')
set(gca,'YLim',[-80 50])

subplot(2,1,2); hold on
plot(time,V(:,end),'k')
xlabel('Time (s)')
ylabel('Voltage in axon (mV)')
set(gca,'YLim',[-80 50])
