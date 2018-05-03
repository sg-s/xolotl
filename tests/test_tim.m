% tests a neuron that reproduces Fig 3 in Tim's paper

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 0; % used only when we add in homeostatic control 

x = xolotl;
x.add('AB','Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca,'Ca_target',Ca_target);

x.AB.add('liu-approx/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu-approx/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu-approx/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu-approx/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu-approx/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu-approx/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu-approx/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

x.t_end = 10e3;
x.integrate;
x.t_end = 1e3;
V = x.integrate;


figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
plot(V)
drawnow