
% this demo shows how we can voltage clamp 
% a single compartment in a multi-compartment neuron
% model 

x = xolotl.examples.neurons.BurstingCable();
x.set('*HCurrent.gbar',0)
x.set('*NaV.gbar',0)
x.dt = .01;
x.sim_dt = .01;

x.closed_loop = false;




V = x.integrate;
time = (1:length(V))*x.dt*1e-3;


% create a sine wave clamp 
V_clamp = sin((1:length(V))/1e4)*30 - 50;
V_clamp = V_clamp + linspace(0,30,length(V_clamp));


% let's clamp the soma
x.V_clamp = V*NaN;
x.V_clamp(:,1) = V_clamp;

data = x.integrate;

I_ext = 0*data;
I_ext(:,1) = data(:,1);

x.I_ext = I_ext;

V = x.integrate;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
plot(time,V_clamp,'k')
plot(time,V(:,1),'r')
xlabel('Time (s)')
ylabel('V_m (mV)')
legend({'Clamped voltage','Voltage when clamped current is injected'},'Location','southeast')
set(gca,'YLim',[-80 20])

figlib.pretty()