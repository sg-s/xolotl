% in this test, first we construct a neuron using
% built-in conductances, and then create
% custom conductances using the conductance class
% and compare that the two neurons are identical

x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');

% try to download a binary
% so that people without compilers can use this too
try
	x.download
end


% construct a new ACurrent based on the Liu ACurrent
[m_inf, h_inf, tau_m, tau_h] =  xolotl.getGatingFunctions(x.AB.ACurrent.cpp_class_path);


% specify a conductance using the function we read out
conductance = xolotl.conductance;
conductance.m_inf = m_inf;
conductance.h_inf = h_inf;
conductance.tau_m = tau_m;
conductance.tau_h = tau_h;
conductance.default_E = x.AB.ACurrent.E;
conductance.p = 3;
conductance.q = 1;

% make a new C++ file
conductance.generateCPPFile('ACurrent');



% now construct a new object using this channels

x2 = xolotl.examples.neurons.BurstingNeuron('prefix','liu');
x2.AB.ACurrent.destroy;
x2.AB.add('custom/ACurrent','gbar',246);


x.sim_dt = .1;
x.dt = .1;
x.reset;
x.approx_channels = 0;
V = x.integrate;
time = (1:length(V))*x.dt*1e-3;

x2.t_end = x.t_end;
x2.sim_dt = .1;
x2.dt = .1;
x2.approx_channels = 0;
x2.reset;
V2 = x2.integrate;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
subplot(2,1,1); hold on
plot(time,V,'k')
title('Using original ACurrent channel')
ylabel('Voltage (mV)')

subplot(2,1,2); hold on
plot(time,V2,'r')
title('Using generated ACurrent channel')
xlabel('Time (s)')
ylabel('Voltage (mV)')

figlib.pretty()
