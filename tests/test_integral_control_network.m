% conversion from Prinz to phi
vol = 1; % this can be anything, doesn't matter
f = 14.96; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.cleanup;
x.addCompartment('AB',-65,0.02,10,0.0628,vol,phi,3000,0.05,tau_Ca,0);

x.addConductance('AB','prinz/NaV',1000,50);
x.addConductance('AB','prinz/CaT',25,30);
x.addConductance('AB','prinz/CaS',60,30);
x.addConductance('AB','prinz/ACurrent',500,-80);
x.addConductance('AB','prinz/KCa',50,-80);
x.addConductance('AB','prinz/Kd',1000,-80);
x.addConductance('AB','prinz/HCurrent',.1,-20);

x.addCompartment('LP',-47,0.01,10,0.0628,vol,phi,3000,0.05,tau_Ca,0);
x.addConductance('LP','prinz/NaV',1000,50);
x.addConductance('LP','prinz/CaS',40,30);
x.addConductance('LP','prinz/ACurrent',200,-80);
x.addConductance('LP','prinz/Kd',250,-80);
x.addConductance('LP','prinz/HCurrent',.5,-20);
x.addConductance('LP','Leak',.3,-50);

x.addCompartment('PY',-41,0.03,10,0.0628,vol,phi,3000,0.05,tau_Ca,0);
x.addConductance('PY','prinz/NaV',1000,50);
x.addConductance('PY','prinz/CaT',25,30);
x.addConductance('PY','prinz/CaS',20,30);
x.addConductance('PY','prinz/ACurrent',500,-80);
x.addConductance('PY','prinz/Kd',1250,-80);
x.addConductance('PY','prinz/HCurrent',.5,-20);
x.addConductance('PY','Leak',.1,-50);

% 2e
x.addSynapse('Glut','AB','LP',0);
x.addSynapse('Glut','AB','PY',10);
x.addSynapse('Glut','LP','PY',1);
x.addSynapse('Glut','PY','LP',30);
% x.addSynapse('Glut','LP','AB',30);


x.dt = 50e-3;
x.t_end = 5000;


x.addIntegralController('synapse',1,3e3,3e3);
x.LP.Ca_target = 43.3;

x.transpile;
x.compile;

x.t_end = 200e3;
x.integrate;


x.t_end = 20e3;
x.integrate;

figure('outerposition',[30 30 1200 900],'PaperUnits','points','PaperSize',[1200 600]); hold on
for i = 1:3
	subplot(2,3,i); hold on
	plot(V(:,i))

	subplot(2,3,i+3); hold on
	plot(Ca(:,i))

end