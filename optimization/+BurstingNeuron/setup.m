% sets up a xfit object to find Bursting neurons
% 
function p = setup()


% first, we create our xolotl object
x = xolotl.examples.BurstingNeuron('prefix','prinz');
x.set('*gbar',veclib.shuffle((x.get('*gbar'))))

% we instantiate the xfit object
% and specify the particle swarm engine
p = xfit('particleswarm');
p.options.UseParallel = true;

% we assign a cost function
p.SimFcn = @xolotl.examples.burstingCostFcn;


% we assign the xolotl object
p.x = x;

% we optimzie over all maximal conductances
p.FitParameters = x.find('*gbar');
% lower bound values
p.lb = [100 0 0 0 0 500 0 500];
% upper bound values
p.ub = [1e3 100 100 10 500 2000 1 2000];

p.SaveParameters = x.find('*gbar');

p.SaveWhenCostBelow = .1;