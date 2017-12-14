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
x.addCompartment('AB',-65,0.02,10,0.0628,vol,phi,3000,0.05,tau_Ca,0);

qg = 2;
qm = 2;
qh = 2;

x.addConductance('AB','prinz/NaV',1000,50,0,1,qg,qm,qh);
x.addConductance('AB','prinz/CaT',25,30,0,1,qg,qm,qh);
x.addConductance('AB','prinz/CaS',60,30,0,1,qg,qm,qh);
x.addConductance('AB','prinz/ACurrent',500,-80,0,1,qg,qm,qh);
x.addConductance('AB','prinz/KCa',50,-80,0,1,qg,qm,qh);
x.addConductance('AB','prinz/Kd',1000,-80,0,1,qg,qm,qh);
x.addConductance('AB','prinz/HCurrent',.1,-20,0,1,qg,qm,qh);


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


