% this test shows off custom manipulate functions, and how
% you can visualize the effects of changing parameters of the f-I curve 
% of a neuron

xolotl.cleanup;

x = xolotl;
x.add('compartment','AB','A',0.0628);
x.AB.add('CalciumMech1','f',1.496);
x.AB.add('liu-approx/NaV','gbar',2e3,'E',30);
x.AB.add('liu-approx/CaT','gbar',10,'E',30);
x.AB.add('liu-approx/Kd','gbar',1e3,'E',-80);
x.AB.add('Leak','gbar', .2,'E',-50);


x.manipulate_plot_func{1} = @custom_fI;
x.manipulate('*gbar');