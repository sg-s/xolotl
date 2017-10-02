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
x.addCompartment('AB',-65,0.02,10,0.0628,vol,phi,3000,0.05,tau_Ca);

x.addConductance('AB','prinz/NaV',1000,50);
x.addConductance('AB','prinz/CaT',25,30);
x.addConductance('AB','prinz/CaS',60,30);
x.addConductance('AB','prinz/ACurrent',500,-80);
x.addConductance('AB','prinz/KCa',50,-80);
x.addConductance('AB','prinz/Kd',1000,-80);
x.addConductance('AB','prinz/HCurrent',.1,-20);

x.addCompartment('LP',-47,0.01,10,0.0628,vol,phi,3000,0.05,tau_Ca);
x.addConductance('LP','prinz/NaV',1000,50);
x.addConductance('LP','prinz/CaS',40,30);
x.addConductance('LP','prinz/ACurrent',200,-80);
x.addConductance('LP','prinz/Kd',250,-80);
x.addConductance('LP','prinz/HCurrent',.5,-20);
x.addConductance('LP','Leak',.3,-50);

x.addCompartment('PY',-41,0.03,10,0.0628,vol,phi,3000,0.05,tau_Ca);
x.addConductance('PY','prinz/NaV',1000,50);
x.addConductance('PY','prinz/CaT',25,30);
x.addConductance('PY','prinz/CaS',20,30);
x.addConductance('PY','prinz/ACurrent',500,-80);
x.addConductance('PY','prinz/Kd',1250,-80);
x.addConductance('PY','prinz/HCurrent',.5,-20);
x.addConductance('PY','Leak',.1,-50);

% % 2a
% x.addSynapse('Chol','AB','LP',30);
% x.addSynapse('Chol','AB','PY',10);
% x.addSynapse('Glut','AB','LP',100);
% x.addSynapse('Glut','AB','PY',30);
% x.addSynapse('Glut','LP','PY',1);
% x.addSynapse('Glut','PY','LP',10);
% x.addSynapse('Glut','LP','AB',30);

% fig 2c
% x.addSynapse('Chol','AB','LP',100);
% x.addSynapse('Chol','AB','PY',30);
% x.addSynapse('Glut','AB','PY',1);
% x.addSynapse('Glut','LP','PY',1);


% fig 2d
% x.addSynapse('Chol','AB','LP',100);
% x.addSynapse('Chol','AB','PY',1);
% x.addSynapse('Glut','AB','LP',3);
% x.addSynapse('Glut','AB','PY',10);
% x.addSynapse('Glut','LP','PY',1);
% x.addSynapse('Glut','PY','LP',10);
% x.addSynapse('Glut','LP','AB',10);


% 2e
x.addSynapse('Chol','AB','LP',30);
x.addSynapse('Chol','AB','PY',3);
x.addSynapse('Glut','AB','LP',30);
x.addSynapse('Glut','AB','PY',10);
x.addSynapse('Glut','LP','PY',1);
x.addSynapse('Glut','PY','LP',30);
x.addSynapse('Glut','LP','AB',30);


% x.transpile;
% x.compile;

x.dt = 50e-3;
x.t_end = 5000;

% [V,Ca] = x.integrate;

% assert(~any(isnan(V(:))),'V contains NaNs. Something is wrong')
% assert(~any(isnan(Ca(:))),'Ca contains NaNs. Something is wrong')

if usejava('jvm')
	x.manipulate;
else
	disp('STG test passed!')
end
