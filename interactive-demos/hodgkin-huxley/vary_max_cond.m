% in this example, we use a version of 
% NaV and Kd channels that expose all their 
% parameters (including that of their activation
% functions, etc) to build an interactive visualization
% that we can manipulate 

if exist('hh_examples.mat','file') == 2
	load('hh_examples.mat')
else

	x = xolotl;
	x.add('compartment','AB','A',0.0628,'vol',.0628);


	x.AB.add('GenericKdLike','Kd','gbar',610);
	x.AB.add('Leak','gbar',.99,'E',-50);
	x.AB.add('GenericNaVLike','NaV','gbar',1831,'E',30);

end

x.AB.NaV.h_tau_B1 = .33;


x.closed_loop = true;
x.t_end = 1e3;
x.integrate;
x.t_end = .5e3;

x.dt = .1;
x.sim_dt = .1;


x.manipulate_plot_func = {@activation_func_plot_func};

x.manipulate([x.find('*gbar'); 'AB.NaV.E'; 'AB.Kd.E'; 'AB.Leak.E'; 'I_ext_AB'])

