% tests a neuron with a process that we 
% then slice into many sub-compartments 

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

x.addCompartment('Neurite',-60,0.02,10,0.1,vol,phi,3000,0.05,tau_Ca,Ca_target);
x.addConductance('Neurite','liu/NaV',1830,30);
x.addConductance('Neurite','liu/CaT',23,30);
x.addConductance('Neurite','liu/CaS',27,30);
x.addConductance('Neurite','liu/ACurrent',246,-80);
x.addConductance('Neurite','liu/KCa',980,-80);
x.addConductance('Neurite','liu/Kd',610,-80);
x.addConductance('Neurite','liu/HCurrent',10.1,-20);
x.addConductance('Neurite','Leak',.99,-50);

