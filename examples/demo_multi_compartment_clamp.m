% this demo shows how we can voltage clamp 
% a single compartment in a multi-compartment neuron
% model 

x = xolotl.examples.BurstingCable();
x.set('*HCurrent.gbar',0)
x.dt = .1;
x.sim_dt = .1;


V = x.integrate;
time = (1:length(V))*x.dt*1e-3;


% create a sine wave clamp 
V_clamp = sin((1:length(V))/1e3)*30 - 50;
V_clamp = V_clamp + linspace(0,30,length(V_clamp));


% let's clamp the soma
x.V_clamp = V*NaN;
x.V_clamp(:,1) = V_clamp;

data = x.integrate;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
subplot(3,1,1); hold on
plot(time,V_clamp,'k')
xlabel('Time (s)')
ylabel('Voltage clamp in soma (mV)')

subplot(3,1,2); hold on
plot(time,data(:,1),'k')
xlabel('Time (s)')
ylabel('Clamping current (nA)')

subplot(3,1,3); hold on
plot(time,data(:,end),'k')
xlabel('Time (s)')
ylabel('Voltage in axon (mV)')
set(gca,'YLim',[-80 50])
