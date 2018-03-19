% tests a neuron that reproduces Fig 3 in Tim's paper

vol = 1; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 7; % used only when we add in homeostatic control 

x = xolotl;
x.cleanup;
x.add('AB','compartment','Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca,'Ca_target',Ca_target);

g0 = 10*rand(7,1);

x.AB.add('liu/NaV','gbar',g0(1),'E',30);
x.AB.add('liu/CaT','gbar',g0(2),'E',30);
x.AB.add('liu/CaS','gbar',g0(3),'E',30);
x.AB.add('liu/ACurrent','gbar',g0(4),'E',-80);
x.AB.add('liu/KCa','gbar',g0(5),'E',-80);
x.AB.add('liu/Kd','gbar',g0(6),'E',-80);
x.AB.add('liu/HCurrent','gbar',g0(7),'E',-20);
x.AB.add('Leak','gbar',.099,'E',-50);

tau_g = 5e3;

x.AB.NaV.add('IntegralController','tau_m',666,'tau_g',tau_g);
x.AB.CaT.add('IntegralController','tau_m',55555,'tau_g',tau_g);
x.AB.CaS.add('IntegralController','tau_m',45454,'tau_g',tau_g);
x.AB.ACurrent.add('IntegralController','tau_m',5000,'tau_g',tau_g);
x.AB.KCa.add('IntegralController','tau_m',1250,'tau_g',tau_g);
x.AB.Kd.add('IntegralController','tau_m',2000,'tau_g',tau_g);
x.AB.HCurrent.add('IntegralController','tau_m',125000,'tau_g',tau_g);

x.transpile;
x.compile;

x.t_end = 100e3;
x.dt = 100e-3;
x.integrate;

x.t_end = 5e3;
V = x.integrate;
plot(V)
