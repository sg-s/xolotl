% this example
% shows how to configure a custom plot_func
% and use it while manipulating parameters


x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');

x.t_end = 1e4;
x.integrate;
x.t_end = 2e3;

x.dt = .1;

x.manipulate_plot_func = {@xolotl.examples.plotfunctions.example};

x.pref.use_polar_gbar = false;

x.manipulate('*gbar')

figlib.pretty('FontSize',14,'LineWidth',1.5)