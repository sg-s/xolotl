% test new conductances

%% Brookings

% set up xolotl object with one compartment
x = xolotl;
x.add('AB', 'compartment', 'Cm', 10, 'A', 0.01);

% add brookings conductances
x.AB.add('brookings/ACurrent', 'gbar', 1, 'E', -80);

figure; plot(x.integrate); hold on;

%% Dethier
clear x
x = xolotl;

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x.add('AB','compartment','Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

% add dethier conductances
% x.AB.add('dethier/CaT', 'gbar', 1);
% x.AB.add('dethier/HCurrent', 'gbar', 1, 'E', -20);
% x.AB.add('dethier/Kd', 'gbar', 1, 'E', -80);
x.AB.add('dethier/NaV', 'gbar', 1, 'E', -50);

plot(x.integrate);
