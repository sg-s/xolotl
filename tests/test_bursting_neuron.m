% this tests generates and integrates
% a bursting neuron neuron that reproduces Fig 3
% in Tim O'Leary's 2013 paper

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 0; % used only when we add in homeostatic control

x = xolotl;
x.add('AB','compartment','Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca,'Ca_target',Ca_target);

x.AB.add('liu/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

x.t_end = 10e3;
x.integrate;
x.t_end = 1e3;
[V, Ca, ~, currents] = x.integrate;

% process the voltage
dV = [0; diff(V)];
Vsign = dV > 0;

curr_index = NaN * Vsign;
[~, curr_index(Vsign)] = min(currents(Vsign,:)');
[~, curr_index(~Vsign)] = max(currents(~Vsign,:)');
unique_currents = unique(curr_index);

cond_names = x.find('conductance');

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
time = 1e-3 * x.dt * (1:length(V));
plot(time, V,'k');
for ii = 1:length(unique_currents)
  Vplot = V;
  Vplot(curr_index ~= unique_currents(ii)) = NaN;
  l(ii) = plot(time, Vplot);
  lgd{ii} = cond_names{unique_currents(ii)};
end
legend(l,lgd)
prettyFig('plw', 4);
ylabel('V_m (mV)')
xlabel('time (s)')
drawnow
