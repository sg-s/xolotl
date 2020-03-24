% test script for matlab wrapper

% this sets up the pyloric network
% as in Fig 2e of this paper:
% Prinz ... Marder Nat Neuro 2004
% http://www.nature.com/neuro/journal/v7/n12/abs/nn1352.html

x = xolotl.examples.networks.pyloric;

x.integrate;


[V, ~, ~, currs, syns] = x.integrate;


C = x.find('compartment');

x.plot;
drawnow

figure('outerposition',[100 100 1000 900],'PaperUnits','points','PaperSize',[1000 900]); hold on
subplot(3,1,1); hold on
plot(currs(:,1:7))
ylabel('I (nA)')
title(C{1})
legend(x.(C{1}).find('conductance'))

subplot(3,1,2); hold on
plot(currs(:,8:15))
title(C{2})
ylabel('I (nA)')
legend(x.(C{2}).find('conductance'))

subplot(3,1,3); hold on
plot(currs(:,16:23))
title(C{3})
ylabel('I (nA)')
legend(x.(C{3}).find('conductance'))


drawnow

figure('outerposition',[100 100 1000 500],'PaperUnits','points','PaperSize',[1000 500]); hold on

plot(syns)
ylabel('I (nA)')
title('synaptic currents')

drawnow


figlib.pretty()