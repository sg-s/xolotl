% this example
% shows how to configure a custom plot_func
% and use it while manipulating parameters


x = xolotl.examples.BurstingNeuron;

x.t_end = 1e4;
x.integrate;
x.t_end = 2e3;

x.dt = .1;

x.manipulate_plot_func = {@example_plot_func};

x.manipulate('*gbar')

