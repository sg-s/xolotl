function p = setup()

% set up the neuron
x = xolotl.examples.TwoCompartmentBurster;

% set up the fitter
p = xfit;
p.x = x;

p.data = twoComp.getData;
p.data = p.data(1);

p.parameter_names = [x.find('*gbar');'Axon.len'; 'CellBody.len'; 'CellBody.radius'; 'CellBody.CalciumMech.f'; 'CellBody.CalciumMech.tau_Ca'];
%      Axon                Soma  
%      A   Kd    L    NaV   A   CaS  CaT  H    KCa  Kd   L
p.lb = [10,  100, 0,  100, 10,  1,   1,   1,   10,  10,  0,  .1, .01, .01, 1,  10];
p.ub = [1e3, 2e3, 10, 3e3, 1e3, 100, 100,  10, 1e3, 2e3, 10,  5,  1,  1,  20, 500];

p.options.UseParallel = true;

p.SimFcn = @twoComp.metricsCost;
p.options.Display = 'iter';

p.SaveParameters = p.parameter_names;