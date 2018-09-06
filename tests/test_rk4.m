

xolotl.cleanup;
close all

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;

x.add('compartment','HH', 'Cm', 10, 'A', 0.01);
x.HH.add('liu/NaV', 'gbar', 1000, 'E', 50);
x.HH.add('liu/Kd', 'gbar', 300, 'E', -80);
x.HH.add('Leak', 'gbar', 1, 'E', -30);


x.t_end = 1e4;
x.integrate;
x.t_end = 1e3;

x.closed_loop = false;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
V = x.integrate;
plot(V,'k')

x.solver_order = 4;
V = x.integrate;
plot(V,'r')
