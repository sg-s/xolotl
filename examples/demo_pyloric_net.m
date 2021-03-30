% test script for matlab wrapper

% this sets up the pyloric network
% as in Fig 2e of this paper:
% Prinz ... Marder Nat Neuro 2004
% http://www.nature.com/neuro/journal/v7/n12/abs/nn1352.html

x = xolotl.examples.networks.pyloric;

% try to download a binary
% so that people without compilers can use this too
try
	x.snapshot('base')
	x.integrate;
	x.reset('base')
catch
	x.download;
end


x.output_type = 1;
data = x.integrate;

C = x.find('compartment');

x.plot;
drawnow



figure('outerposition',[100 100 1000 900],'PaperUnits','points','PaperSize',[1000 900]); hold on

for i = 1:3

	subplot(3,1,i); hold on
	plot(x.time,xolotl.filterOutputData(data.(C{i}),'I')*x.(C{i}).A)
	ylabel('I (nA)')
	title(C{i})
	legend(x.(C{i}).find('conductance'))

end

drawnow

figure('outerposition',[100 100 1000 500],'PaperUnits','points','PaperSize',[1000 500]); hold on
S = x.find('synapse');
for i = 1:length(S)
	temp = structlib.read(data,[S{i} '.state']);
	plot(x.time,temp(:,2))
end
ylabel('I (nA)')
title('synaptic currents')
legend(S)

drawnow


figlib.pretty()