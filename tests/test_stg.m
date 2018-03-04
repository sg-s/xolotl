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

x.addConductance('AB','prinz-fast/NaV',1000,50);
x.addConductance('AB','prinz-fast/CaT',25,30);
x.addConductance('AB','prinz-fast/CaS',60,30);
x.addConductance('AB','prinz-fast/ACurrent',500,-80);
x.addConductance('AB','prinz-fast/KCa',50,-80);
x.addConductance('AB','prinz-fast/Kd',1000,-80);
x.addConductance('AB','prinz-fast/HCurrent',.1,-20);

x.addCompartment('LP',-47,0.01,10,0.0628,vol,phi,3000,0.05,tau_Ca,0);
x.addConductance('LP','prinz-fast/NaV',1000,50);
x.addConductance('LP','prinz-fast/CaS',40,30);
x.addConductance('LP','prinz-fast/ACurrent',200,-80);
x.addConductance('LP','prinz-fast/Kd',250,-80);
x.addConductance('LP','prinz-fast/HCurrent',.5,-20);
x.addConductance('LP','Leak',.3,-50);

x.addCompartment('PY',-41,0.03,10,0.0628,vol,phi,3000,0.05,tau_Ca,0);
x.addConductance('PY','prinz-fast/NaV',1000,50);
x.addConductance('PY','prinz-fast/CaT',25,30);
x.addConductance('PY','prinz-fast/CaS',20,30);
x.addConductance('PY','prinz-fast/ACurrent',500,-80);
x.addConductance('PY','prinz-fast/Kd',1250,-80);
x.addConductance('PY','prinz-fast/HCurrent',.5,-20);
x.addConductance('PY','Leak',.1,-50);

% 2e
x.addSynapse('Chol','AB','LP',30);
x.addSynapse('Chol','AB','PY',3);
x.addSynapse('Glut','AB','LP',30);
x.addSynapse('Glut','AB','PY',10);
x.addSynapse('Glut','LP','PY',1);
x.addSynapse('Glut','PY','LP',30);
x.addSynapse('Glut','LP','AB',30);


x.dt = 50e-3;
x.t_end = 20e3;

x.transpile;
x.compile;

V = x.integrate;

figure('outerposition',[0 0 1000 900],'PaperUnits','points','PaperSize',[1000 900]); hold on
for i = 1:3
	subplot(3,1,i); hold on
	plot(V(:,i))
end