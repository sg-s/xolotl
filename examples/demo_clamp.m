%
% In this demo, we set up a neuron model with a single conductance,
% and simulate a voltage clamp experiment to extract a I-V curve from this
% neuron. We then treat this I-V curve as something an experimenter
% would have given us, and back-calculate the activation curve of that channel


% model setup
x = xolotl;
x.add('compartment','AB','A',0.0628);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

% test showing the model
x

% try to download a binary
% so that people without compilers can use this too
try
	x.snapshot('base')
	x.integrate;
	x.reset('base')
catch
	x.download;
end


% make a note of what the actual activation curve is
f = x.AB.Kd.cpp_child_functions(1).fun_handle;
all_V_step = linspace(-80,50,30);
actual_m_inf = NaN*all_V_step;
for i = 1:length(all_V_step)
	actual_m_inf(i) = f(all_V_step(i),0);
end


% specify what experiment we're doing to do
holding_V = -60;

x.t_end = 5e2;

all_I = NaN(x.t_end/x.dt,length(all_V_step));
all_V_step = repmat(all_V_step,x.t_end/x.dt,1);

x.V_clamp = repmat(holding_V,x.t_end/x.dt,1);


x.integrate;
x.closed_loop = false;

for i = 1:size(all_V_step,2)
	x.V_clamp = all_V_step(:,i);
	all_I(:,i) = x.integrate;
end

V = all_V_step(end,:);
I = all_I(end,:);

time = (1:length(all_I))*x.dt;

figure('outerposition',[300 300 1200 1200],'PaperUnits','points','PaperSize',[1200 1200]); hold on
subplot(2,2,1); hold on
c = parula(size(all_V_step,2));
for i = 1:size(all_V_step,2)
	plot(time,all_I(:,i),'Color',c(i,:))
end
xlabel('Time (ms)')
ylabel('Current (nA)')
set(gca,'XScale','log')

% plot I-V curve
subplot(2,2,2); hold on
plot(V,I,'r')
xlabel('Voltage step (mV)')
ylabel('Current (nA)')


% estimate conductance from I-V curve
g_est = I./(V-x.AB.Kd.E);
g_est(g_est<0) = 0;
subplot(2,2,3); hold on
plot(V,g_est,'r')
xlabel('Voltage (mV)')
ylabel('Estimated conductance')


% normalize g_est and compare to actual activation curve
g_est = g_est/g_est(end);
subplot(2,2,4); hold on

% compare the estimated conductance and the actual curve
% note that we had to raise the conductance to 1/4 power
% because there are 4 sub-units in the K channel
plot(V,actual_m_inf,'k')
plot(V,g_est,'r');
plot(V,g_est.^(1/4),'b');
legend({'Actual m_{inf}','normalized g','g^{1/4}'},'Location','southeast')
xlabel('Voltage (mV)')
figlib.pretty


% So it looks OK
% somethings we did not consider here:
% 1. the leak conductance. If you want to do this properly, you need to 
% subtract the leak conductance
% 2. You want to find the exponent (here, 4) algorithmically

drawnow
