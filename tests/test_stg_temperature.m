% test script for matlab wrapper 

% this sets up the STG network 
% as in Fig 2e of this paper:
% Prinz ... Marder Nat Neuro 2004
% http://www.nature.com/neuro/journal/v7/n12/abs/nn1352.html


% conversion from Prinz to phi
vol = 1; % this can be anything, doesn't matter
f = 14.96; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.cleanup;
x.add('AB','compartment','V',-65,'Ca',0.02,'Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

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

x.transpile;
x.compile;

x.closed_loop = false;

V = x.integrate;

x.temperature = 22;
V2 = x.integrate;

figure('outerposition',[0 0 1000 500],'PaperUnits','points','PaperSize',[1000 500]); hold on

plot(V,'k')
plot(V2,'r')


drawnow