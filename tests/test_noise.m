vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.add('compartment','AB','Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

x.AB.add('liu/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

x.AB.add('CurrentNoise','noise_amplitude',2);

x.sim_dt = .1;
x.dt = .1;

x.integrate;



x.t_end = 2e3;

x.snapshot('zero');

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
V = x.integrate;
time = (1:length(V))*x.dt*1e-3;
plot(time,V,'k')

x.reset('zero')
V = x.integrate;
plot(time,V,'r')

xlabel('Time (s)')