% tests a neuron that reproduces Fig 3 in Tim's paper

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 0; % used only when we add in homeostatic control 

x = xolotl;
x.add('AB',cpplab('compartment','V',-60,'Ca',0.02,'Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca,'Ca_target',Ca_target));

% set up a relational parameter
x.AB.vol = @() x.AB.A;

x.AB.add('NaV',cpplab('liu-approx/NaV','gbar',@() 115/x.AB.A,'E',30));
x.AB.add('CaT',cpplab('liu-approx/CaT','gbar',@() 1.7/x.AB.A,'E',30));
x.AB.add('CaS',cpplab('liu-approx/CaS','gbar',@() 1.7/x.AB.A,'E',30));
x.AB.add('ACurrent',cpplab('liu-approx/ACurrent','gbar',@() 1.7/x.AB.A,'E',30));
x.AB.add('KCa',cpplab('liu-approx/KCa','gbar',@() 1.7/x.AB.A,'E',30));
x.AB.add('Kd',cpplab('liu-approx/Kd','gbar',@() 1.7/x.AB.A,'E',30));
x.AB.add('HCurrent',cpplab('liu-approx/HCurrent','gbar',@() 1.7/x.AB.A,'E',30));
x.AB.add('Leak',cpplab('Leak','gbar',@() 1.7/x.AB.A,'E',30));



% x.transpile;
% x.compile;

% V = x.integrate;
% plot(V)