% this script shows some nice two-compartment 
% bursting neuron models that were fit to real data
% using xfit 
% This model is supposed to mimic a bursting neuron
% in the pyloric circuit of the crab STG


x = xolotl.examples.neurons.TwoCompartmentBurster;

% add some noise
x.CellBody.add('CurrentNoise','noise_amplitude',.5);

d = fileparts(mfilename('fullpath'));

% load data results
load([d filesep 'twoComp.xfit'],'-mat');


% remove duplicates
[all_params, idx] = unique(all_params,'rows');
all_cost = all_cost(idx);
all_hash = all_hash(idx);

parameter_names = {'Axon.ACurrent.gbar', 'Axon.Kd.gbar', 'Axon.Leak.gbar', 'Axon.NaV.gbar', 'CellBody.ACurrent.gbar', 'CellBody.CaS.gbar', 'CellBody.CaT.gbar', 'CellBody.HCurrent.gbar', 'CellBody.KCa.gbar', 'CellBody.Kd.gbar', 'CellBody.Leak.gbar', 'Axon.len', 'CellBody.len', 'CellBody.radius'   , 'CellBody.CalciumMech.f', 'CellBody.CalciumMech.tau_Ca'};

% show some nice ones

show_these = [714 579 967 244 1173 760 1161 115 765 445 775 1177 635 945 51 886 837 407 802 413 924 193 1072];

figure('outerposition',[300 300 801 1100],'PaperUnits','points','PaperSize',[801 1100]); hold on




for i = 1:18
	subplot(6,3,i); hold on

	x.set(parameter_names,all_params(show_these(i),:))
	x.closed_loop = true;
	x.reset;
	x.integrate;
	V = x.integrate;
	time = (1:length(V))*x.dt*1e-3;
	plot(time,V(:,2)+ randn(length(V),1)*5e-2,'k')


	axis off
end

figlib.pretty('PlotLineWidth',1)

