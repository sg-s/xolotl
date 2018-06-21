% test new conductances

%% Brookings

% set up xolotl object with one compartment
x = xolotl;
x.add('AB', 'compartment', 'Cm', 10, 'A', 0.01);

% add brookings conductances
x.AB.add('brookings/ACurrent', 'gbar', 1, 'E', -80);
