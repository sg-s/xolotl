% tests a neuron that reproduces Fig 3 in Tim's paper

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 0; % used only when we add in homeostatic control 

x = xolotl;
x.addCompartment('AB',-60,0.02,10,0.0628,vol,phi,3000,0.05,tau_Ca,Ca_target);

% set up a relational parameter
x.AB.vol = @() x.AB.A;

x.addConductance('AB','liu-approx/NaV',@() 115/x.AB.A,30);
x.addConductance('AB','liu-approx/CaT',@() 1.44/x.AB.A,30);
x.addConductance('AB','liu-approx/CaS',@() 1.7/x.AB.A,30);
x.addConductance('AB','liu-approx/ACurrent',@() 15.45/x.AB.A,-80);
x.addConductance('AB','liu-approx/KCa',@() 61.54/x.AB.A,-80);
x.addConductance('AB','liu-approx/Kd',@() 38.31/x.AB.A,-80);
x.addConductance('AB','liu-approx/HCurrent',@() .6343/x.AB.A,-20);
x.addConductance('AB','Leak',@() 0.0622/x.AB.A,-50);

x.transpile;
x.compile;

V = x.integrate;
plot(V)