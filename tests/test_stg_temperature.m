% test script for matlab wrapper 

% this sets up the STG network 
% as in Fig 2e of this paper:
% Prinz ... Marder Nat Neuro 2004
% http://www.nature.com/neuro/journal/v7/n12/abs/nn1352.html

xolotl.cleanup;



x = xolotl;
x.add('compartment','AB','A',0.0628);

x.AB.add('CalciumMech');

qg = 2;
qm = 2;
qh = 2;

x.AB.add('prinz-temperature/NaV','gbar',1000,'E',50,'Q_g',qg,'Q_tau_m',qm,'Q_tau_h',qh);
x.AB.add('prinz-temperature/CaT','gbar',25,'E',30,'Q_g',qg,'Q_tau_m',qm,'Q_tau_h',qh);
x.AB.add('prinz-temperature/CaS','gbar',60,'E',30,'Q_g',qg,'Q_tau_m',qm,'Q_tau_h',qh);
x.AB.add('prinz-temperature/ACurrent','gbar',500,'E',-80,'Q_g',qg,'Q_tau_m',qm,'Q_tau_h',qh);
x.AB.add('prinz-temperature/KCa','gbar',50,'E',-80,'Q_g',qg,'Q_tau_m',qm);
x.AB.add('prinz-temperature/Kd','gbar',1000,'E',-80,'Q_g',qg,'Q_tau_m',qm);
x.AB.add('prinz-temperature/HCurrent','gbar',.1,'E',-20,'Q_g',qg,'Q_tau_m',qm);


x.dt = 50e-3;
x.t_end = 20e3;

x.closed_loop = false;

V = x.integrate;

x.temperature = 22;
V2 = x.integrate;

figure('outerposition',[0 0 1000 500],'PaperUnits','points','PaperSize',[1000 500]); hold on

plot(V,'k')
plot(V2,'r')


drawnow