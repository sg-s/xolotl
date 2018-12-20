% in this example, we use a version of 
% NaV and Kd channels that expose all their 
% parameters (including that of their activation
% functions, etc) to build an interactive visualization
% that we can manipulate 




x = xolotl;
x.add('compartment','AB','A',0.0628,'vol',.0628);


x.AB.add('GenericKdLike','Kd','gbar',610);
x.AB.add('Leak','gbar',.99,'E',-50);
x.AB.add('GenericNaVLike','NaV','gbar',1831,'E',30);




x.t_end = 1e3;
x.integrate;
x.t_end = .5e3;

x.dt = .1;


x.manipulate_plot_func = {@activation_func_plot_func};

x.manipulate(unique([x.find('*tau*'); x.find('*V_*')]))

