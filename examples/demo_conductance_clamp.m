
%% demo_conductance_clamp


close all

x = xolotl.examples.neurons.BurstingNeuron();

% try to download a binary
% so that people without compilers can use this too
try
    x.snapshot('base')
    x.integrate;
    x.reset('base')
catch
    x.download;
end


x.sim_dt = .1;
x.dt = .1;

x.t_end = 5e3;
x.integrate;

x.AB.NaV.add('ConductanceClamp')

x.GPData = linspace(500,1000, x.t_end/x.sim_dt)';

V = x.integrate;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on

subplot(2,1,1); hold on
plot(x.time,x.GPData)
ylabel('gbar_{NaV}')

subplot(2,1,2); hold on
plot(x.time,V)
ylabel('V_m (mV)')
xlabel('Time (s)')

figlib.pretty('PlotLineWidth',1.5,'LineWidth',1.5)