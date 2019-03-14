
close all
clear all
rehash
cpplab.rebuildCache;
xolotl.cleanup;

x = xolotl;
x.add('compartment','AB','A',0.0628,'vol',.0628);
x.AB.add('prinz/CalciumMech','f',14.96);


x.AB.add(['liu/NaV'],'gbar',2e3,'E',30);
x.AB.add(['liu/ACurrent'],'gbar',657,'E',-80);
x.AB.add(['liu/KCa'],'gbar',261,'E',-80);
x.AB.add(['liu/Kd'],'gbar',790,'E',-80);
x.AB.add(['liu/HCurrent'],'gbar',47,'E',-20);
x.AB.add('Leak','gbar',.087,'E',-50);


x.closed_loop = true;
x.sim_dt = .05;
x.dt = .1;

x.t_end = 1e6;

x.transpile;
x.compile;

disp('Successfully compiled. Starting integration...')



x.t_end = 2e3; V = x.integrate;


figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
plot(V,'k')

figlib.pretty('plw',1.5,'lw',1.5)