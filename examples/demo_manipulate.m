% this example
% shows how to configure a custom plot_func
% and use it while manipulating parameters


vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.add('compartment','AB','A',0.0628,'vol',.0628);
x.AB.add('bucholtz/CalciumMech','phi',phi);



x.AB.add('liu/ACurrent','gbar',246);
x.AB.add('liu/CaS','gbar',27);
x.AB.add('liu/CaT','gbar',23);
x.AB.add('liu/HCurrent','gbar',10,'E',-20);
x.AB.add('liu/KCa','gbar',980);
x.AB.add('liu/Kd','gbar',610);
x.AB.add('Leak','gbar',.99,'E',-50);
x.AB.add('liu/NaV','gbar',1831,'E',30);




x.t_end = 1e4;
x.integrate;
x.t_end = 2e3;

x.dt = .1;


x.manipulate_plot_func = {@example_plot_func};

x.manipulate('*gbar')

