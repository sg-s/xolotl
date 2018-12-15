% test script for matlab wrapper

% this sets up the STG network
% as in Fig 2e of this paper:
% Prinz ... Marder Nat Neuro 2004
% http://www.nature.com/neuro/journal/v7/n12/abs/nn1352.html


% conversion from Prinz to phi
A = 0.0628;

channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};
prefix = 'prinz/';
gbar(:,1) = [1000 25  60 500  50  1000 .1];
gbar(:,2) = [1000 0   40 200  0   250  .5];
gbar(:,3) = [1000 24  20 500  0   1250 .5];
E =         [50   30  30 -80 -80 -80   -20];

x = xolotl;

x.add('compartment','AB','Cm',10,'A',A);
x.add('compartment','LP','Cm',10,'A',A);
x.add('compartment','PY','Cm',10,'A',A);

compartments = x.find('compartment');
for j = 1:length(compartments)

	% add Calcium mechanism
	x.(compartments{j}).add('prinz/CalciumMech');

	for i = 1:length(channels)
		x.(compartments{j}).add([prefix channels{i}],'gbar',gbar(i,j),'E',E(i));
	end
end

x.LP.add('Leak','gbar',.3,'E',-50);
x.PY.add('Leak','gbar',.1,'E',-50);

x.connect('AB','LP','Chol');

% set up synapses as in Fig. 2e

x.connect('AB','PY','Chol','gmax',3);
x.connect('AB','LP','Glut','gmax',30);
x.connect('AB','PY','Glut','gmax',10);
x.connect('LP','PY','Glut','gmax',1);
x.connect('PY','LP','Glut','gmax',30);
x.connect('LP','AB','Glut','gmax',30);

x.t_end = 5e3;




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
