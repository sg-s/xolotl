% this script shows some nice two-compartment 
% bursting neuron models that were fit to real data
% using xfit 
% This model is supposed to mimic a bursting neuron
% in the pyloric circuit of the crab STG


x = xolotl.examples.neurons.TwoCompartmentBurster;

% add some noise
x.CellBody.add('CurrentNoise','noise_amplitude',.5);


% try to download a binary
% so that people without compilers can use this too
try
	x.snapshot('base')
	x.integrate;
	x.reset('base')
catch
	x.download;
end



d = fileparts(mfilename('fullpath'));

% load data results
load([d filesep 'twoComp.xfit'],'-mat');


% remove duplicates
[all_params, idx] = unique(all_params,'rows');
all_cost = all_cost(idx);
all_hash = all_hash(idx);

FitParameters = {'Axon.ACurrent.gbar', 'Axon.Kd.gbar', 'Axon.Leak.gbar', 'Axon.NaV.gbar', 'CellBody.ACurrent.gbar', 'CellBody.CaS.gbar', 'CellBody.CaT.gbar', 'CellBody.HCurrent.gbar', 'CellBody.KCa.gbar', 'CellBody.Kd.gbar', 'CellBody.Leak.gbar', 'Axon.len', 'CellBody.len', 'CellBody.radius'   , 'CellBody.CalciumMech.f', 'CellBody.CalciumMech.tau_Ca'};

% show some nice ones

show_these = [579 244 760 765 445 775 1177 635 945 51 886 407 802 413 924 193 1072];

figure('outerposition',[300 300 1211 800],'PaperUnits','points','PaperSize',[1211 800]); hold on


try
	load all_PD

	for i = 1:4
		subplot(5,2,i); hold on
		time = (1:length(all_PD))*1e-4;
		plot(time,all_PD(:,i),'k')
		set(gca,'XLim',[0 3])
		axis off
	end
	
catch
end



for i = 1:6
	subplot(5,2,i+4); hold on


	x.t_end = 10e3;
	x.set(FitParameters,all_params(show_these(i),:))
	x.closed_loop = true;
	x.reset;
	x.integrate;
	x.t_end = 3e3;
	V = x.integrate;
	time = (1:length(V))*x.dt*1e-3;
	plot(time,V(:,2)+ randn(length(V),1)*5e-2,'k')


	axis off
end

figlib.pretty('PlotLineWidth',1)

