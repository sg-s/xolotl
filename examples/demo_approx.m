

xolotl.cleanup;
close all


% conversion from Prinz to phi
A = 0.0628;

channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};
prefix = 'prinz/';
gbar(:,1) = [1000 25  60 500  50  1000 .1];
E =         [50   30  30 -80 -80 -80   -20];

x = xolotl;

x.add('compartment','AB','Cm',10,'A',A);

% add Calcium mechanism
x.AB.add('prinz/CalciumMech');

for i = 1:length(channels)
	x.AB.add([prefix channels{i}],'gbar',gbar(i),'E',E(i));
end

x.t_end = 1e4;
x.integrate;



x.closed_loop = false;
x.sim_dt = .05;
x.dt = .1;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
x.t_end = 100e3; x.sim_dt = .05; tic; V = x.integrate; t =  toc;
x.t_end = 5e3; V = x.integrate;
disp(['Exact speed = ' oval(100/t) 'X'])
plot(V,'k')

x.approx_channels = 1;
x.t_end = 100e3; x.sim_dt = .05; tic; V = x.integrate; t =  toc;
x.t_end = 5e3; V = x.integrate;
disp(['Approximate speed = ' oval(100/t) 'X'])
plot(V,'r')
legend({'Exact','Approximate'})
