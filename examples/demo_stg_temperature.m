% test script for matlab wrapper

% this sets up the STG network
% as in Fig 2e of this paper:
% Prinz ... Marder Nat Neuro 2004
% http://www.nature.com/neuro/journal/v7/n12/abs/nn1352.html



x = xolotl;
x.add('compartment','AB','A',0.0628);

x.AB.add('prinz/CalciumMech');

x.AB.add('prinz-temperature/NaV','gbar',1000);
x.AB.add('prinz-temperature/CaT','gbar',25);
x.AB.add('prinz-temperature/CaS','gbar',60);
x.AB.add('prinz-temperature/ACurrent','gbar',500);
x.AB.add('prinz-temperature/KCa','gbar',50);
x.AB.add('prinz-temperature/Kd','gbar',1000);
x.AB.add('prinz-temperature/HCurrent','gbar',.1);

x.set('*Q_g',2)
x.set('*Q_h',2)
x.set('*Q_tau_m',2)
x.set('*Q_tau_h',2)

x.dt = 50e-3;
x.t_end = 5e3;

x.closed_loop = false;

V = x.integrate;
time = (1:length(V))*1e-3*x.dt;

x.temperature = 22;
V2 = x.integrate;

figure('outerposition',[0 0 1000 500],'PaperUnits','points','PaperSize',[1000 500]); hold on

plot(time,V,'k')
plot(time,V2,'r')
xlabel('Time (s)')
ylabel('V_{m} (mV)')
legend({'11C','22C'})
set(gca,'XLim',[0 6],'YLim',[-80 50])
prettyFig('plw',1,'lw',1)
drawnow
