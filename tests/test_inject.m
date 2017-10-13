x = xolotl;
x.closed_loop = false;

% conversion from Prinz to phi
vol = 1; % this can be anything, doesn't matter
f = 14.96; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

% AB neuron
x.addCompartment('AB',-65,0.02,10,0.0628,vol,phi,3000,0.05,tau_Ca,0);
x.addConductance('AB','prinz/NaV',0,50);
x.addConductance('AB','prinz/CaT',0,30);
x.addConductance('AB','prinz/CaS',189,30);
x.addConductance('AB','prinz/ACurrent',1589,-80);
x.addConductance('AB','prinz/KCa',323,-80);
x.addConductance('AB','prinz/Kd',8142,-80);
x.addConductance('AB','prinz/HCurrent',1,-20);
x.addConductance('AB','MICurrent',0,0);
x.addCurrent('AB',0*linspace(x.dt,x.t_end,x.t_end/x.dt));

x.transpile;
x.compile;
