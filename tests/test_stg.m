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
x.AB.add('prinz-approx/NaV','gbar',1000,'E',50);
x.AB.add('prinz-approx/CaT','gbar',25,'E',30);
x.AB.add('prinz-approx/CaS','gbar',60,'E',30);
x.AB.add('prinz-approx/ACurrent','gbar',500,'E',-80);
x.AB.add('prinz-approx/KCa','gbar',50,'E',-80);
x.AB.add('prinz-approx/Kd','gbar',1000,'E',-80);
x.AB.add('prinz-approx/HCurrent','gbar',.1,'E',-20);

x.add('LP','compartment','V',-47,'Ca',0.02,'Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);
x.LP.add('prinz-approx/NaV','gbar',1000,'E',50);
x.LP.add('prinz-approx/CaS','gbar',40,'E',30);
x.LP.add('prinz-approx/ACurrent','gbar',200,'E',-80);
x.LP.add('prinz-approx/Kd','gbar',250,'E',-80);
x.LP.add('prinz-approx/HCurrent','gbar',.5,'E',-20);
x.LP.add('Leak','gbar',.3,'E',-50);

x.add('PY','compartment','V',-47,'Ca',0.02,'Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);
x.PY.add('prinz-approx/NaV','gbar',1000,'E',50);
x.PY.add('prinz-approx/CaT','gbar',25,'E',30);
x.PY.add('prinz-approx/CaS','gbar',20,'E',30);
x.PY.add('prinz-approx/ACurrent','gbar',500,'E',-80);
x.PY.add('prinz-approx/Kd','gbar',1250,'E',-80);
x.PY.add('prinz-approx/HCurrent','gbar',.5,'E',-20);
x.PY.add('Leak','gbar',.1,'E',-50);

% set up synapses as in Fig. 2e
x.connect('AB','LP','Chol','gbar',30);
x.connect('AB','PY','Chol','gbar',3);
x.connect('AB','LP','Glut','gbar',30);
x.connect('AB','PY','Glut','gbar',10);
x.connect('LP','PY','Glut','gbar',1);
x.connect('PY','LP','Glut','gbar',30);
x.connect('LP','AB','Glut','gbar',30);

x.t_end = 20e3;

x.transpile;
x.compile;

V = x.integrate;

figure('outerposition',[0 0 1000 900],'PaperUnits','points','PaperSize',[1000 900]); hold on
for i = 1:3
	subplot(3,1,i); hold on
	plot(V(:,i))
end
drawnow