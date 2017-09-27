% test script for matlab wrapper 

% this sets up a Prinzian network
% AB and PD are separate cells


x = xolotl;

% AB neuron
x.addCompartment('AB',-70,0.05,10,0.0628,14.96,3000,0.05,200);

x.addConductance('AB','prinz/NaV',0,50);
x.addConductance('AB','prinz/CaT',0,30);
x.addConductance('AB','prinz/CaS',189,30);
x.addConductance('AB','prinz/ACurrent',1589,-80);
x.addConductance('AB','prinz/KCa',323,-80);
x.addConductance('AB','prinz/Kd',8142,-80);
x.addConductance('AB','prinz/HCurrent',1,-20);
x.addConductance('AB','MICurrent',0,0);

x.addCompartment('PD',-70,0.05,10,0.0628,14.96,3000,0.05,200);

x.addConductance('PD','prinz/NaV',1000,50);
x.addConductance('PD','prinz/CaT',25,30);
x.addConductance('PD','prinz/CaS',20,30);
x.addConductance('PD','prinz/ACurrent',500,-80);
x.addConductance('PD','prinz/Kd',1250,-80);
x.addConductance('PD','prinz/HCurrent',.5,-20);
x.addConductance('PD','Leak',.1,-50);
x.addConductance('PD','MICurrent',0,0);


x.addCompartment('LP',-70,0.01,10,0.0628,14.96,3000,0.05,200);
x.addConductance('LP','prinz/NaV',1000,50);
x.addConductance('LP','prinz/CaS',40,30);
x.addConductance('LP','prinz/ACurrent',200,-80);
x.addConductance('LP','prinz/Kd',250,-80);
x.addConductance('LP','prinz/HCurrent',.5,-20);
x.addConductance('LP','Leak',.3,-50);
x.addConductance('LP','MICurrent',0,0);

x.addCompartment('PY',-70,0.03,10,0.0628,14.96,3000,0.05,200);
x.addConductance('PY','prinz/NaV',1000,50);
x.addConductance('PY','prinz/CaT',25,30);
x.addConductance('PY','prinz/CaS',20,30);
x.addConductance('PY','prinz/ACurrent',500,-80);
x.addConductance('PY','prinz/Kd',1250,-80);
x.addConductance('PY','prinz/HCurrent',.5,-20);
x.addConductance('PY','Leak',.1,-50);
x.addConductance('PY','MICurrent',0,0);

x.addSynapse('Chol','PD','LP',30);
x.addSynapse('Chol','PD','PY',30);
x.addSynapse('Glut','AB','LP',30);
x.addSynapse('Glut','AB','PY',30);
x.addSynapse('Glut','LP','PY',0.1);
x.addSynapse('Glut','PY','LP',30);
x.addSynapse('Glut','LP','PD',30);
x.addSynapse('Elec','AB','PD',30);
x.addSynapse('Elec','PD','AB',30);

% x.transpile;
% x.compile;

x.dt = 50e-3;
x.t_end = 5000;

[V,Ca] = x.integrate;

assert(~any(isnan(V(:))),'V contains NaNs. Something is wrong')
assert(~any(isnan(Ca(:))),'Ca contains NaNs. Something is wrong')

if usejava('jvm')
	x.manipulate;
else
	disp('STG test passed!')
end

