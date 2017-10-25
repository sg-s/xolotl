% tests a neuron that reproduces Fig 3 in Tim's paper

vol = 1; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 0; % used only when we add in homeostatic control 

x = xolotl;
x.addCompartment('AB',-60,0.02,10,0.0628,vol,phi,3000,0.05,tau_Ca,Ca_target);

x.addConductance('AB','liu/NaV',1830,30);
x.addConductance('AB','liu/CaT',23,30);
x.addConductance('AB','liu/CaS',27,30);
x.addConductance('AB','liu/ACurrent',246,-80);
x.addConductance('AB','liu/KCa',980,-80);
x.addConductance('AB','liu/Kd',610,-80);
x.addConductance('AB','liu/HCurrent',10.1,-20);
x.addConductance('AB','Leak',.99,-50);

if usejava('jvm')
	x.manipulate;
else
	sp = xolotl.findSpikes(x.integrate);
	assert(length(sp)>10,'Not enough spikes')
	disp('Test Passed')
end