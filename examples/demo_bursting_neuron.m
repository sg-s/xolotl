
%% demo_bursting_neuron
% this examples how to generate a simple bursting neuron
% model, and use the plot function to display its voltage
% trace


x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');

% try to download a binary
% so that people without compilers can use this too
try
	x.snapshot('base')
	x.integrate;
	x.reset('base')
catch
	x.download;
end


x.t_end = 1e3;
x.integrate;

x.pref.plot_color = true;

x.plot;
