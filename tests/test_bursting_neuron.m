% this tests generates and integrates
% a bursting neuron neuron that reproduces Fig 3
% in Tim O'Leary's 2013 paper


xolotl.cleanup;


x = xolotl;
x.add('compartment','AB','A',0.0628);
x.AB.add('CalciumMech','f',1.496);

x.AB.add('liu/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

x.t_end = 1e4;
x.integrate;
x.t_end = 1e3;

x.plot;
prettyFig();

x.benchmark;