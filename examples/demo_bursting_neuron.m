
%% demo_bursting_neuron
% this examples how to generate a simple bursting neuron
% model, and use the plot function to display its voltage
% trace


x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');

x.t_end = 1e3;
x.integrate;
x.plot;
