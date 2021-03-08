
close all

% tests a neuron that reproduces Fig 3 in Tim O'Leary's paper

x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');

g0 = 1e-1+1e-1*rand(8,1);
x.set('*gbar',g0);
x.AB.Leak.gbar = .099;

% configure the target and error rule
x.AB.add('InstCalciumError','Target',7)


x.AB.add('CalciumSensor');

x.AB.NaV.add('oleary/IntegralController','tau_m',666);
x.AB.CaT.add('oleary/IntegralController','tau_m',55555);
x.AB.CaS.add('oleary/IntegralController','tau_m',45454);
x.AB.ACurrent.add('oleary/IntegralController','tau_m',5000);
x.AB.KCa.add('oleary/IntegralController','tau_m',1250);
x.AB.Kd.add('oleary/IntegralController','tau_m',2000);
x.AB.HCurrent.add('oleary/IntegralController','tau_m',125000);

% try to download a binary
% so that people without compilers can use this too
try
	x.snapshot('base')
	x.integrate;
	x.reset('base')
catch
	x.download;
end

x.t_end = 5e5;
x.sim_dt = .1;
x.dt = 100;

x.output_type = 1;

data = x.integrate;
g = xolotl.filterOutputData(data,'IntegralController');
g = g(:,2:2:end);

Ca = xolotl.filterOutputData(data,'CalciumSensor');
Ca = Ca./x.AB.InstCalciumError.Target;

figure('outerposition',[300 300 900 901],'PaperUnits','points','PaperSize',[1200 901]); hold on


subplot(3,1,1); hold on

time = x.dt*(1:length(Ca))*1e-3;
plot(time,1+time*0,'k--');
plot(time,Ca,'r','LineWidth',2);
set(gca,'XScale','log','YScale','log')
xlabel('Time (s)')
ylabel('<Ca>/Ca_T ')


subplot(3,1,2); hold on

time = x.dt*(1:length(g))*1e-3;
plot(time,g);
set(gca,'XScale','log','YScale','log','YTick',[1e-2 1e0 1e2 1e4])
xlabel('Time (s)')
ylabel('g (uS/mm^2)')



x.output_type = 0;

subplot(3,1,3); hold on
x.dt = .1;
x.t_end = 1e3;
V = x.integrate;
time = x.dt*(1:length(V))*1e-3;
plot(time,V,'k')
set(gca,'YLim',[-80 50])
ylabel('V_m (mV)')
xlabel('Time (s)')
drawnow

figlib.pretty('PlotLineWidth',1.5,'LineWidth',1.5)
