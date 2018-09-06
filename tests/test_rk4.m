

xolotl.cleanup;
close all

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;

x.add('compartment','AB','A',0.0628,'vol',.0628);
x.AB.add('CalciumMech2','phi',phi);


x.AB.add('liu/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);




x.t_end = 1e4;
x.integrate;
x.t_end = 1e3;

x.closed_loop = false;

% figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
% V = x.integrate;
% plot(V,'k')

x.solver_order = 4;
x.sim_dt = .01;
x.dt = .01;
% x.t_end = 1;
% x.verbosity = 99;
V = x.integrate;
plot(V,'r')
