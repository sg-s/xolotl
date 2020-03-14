%
% __   ____ _ _ __(_) __| |
% \ \/ / _` | '__| |/ _` |
%  >  < (_| | |  | | (_| |
% /_/\_\__, |_|  |_|\__,_|
%      |___/
%
% ### benchmark
%
%
% **Syntax**
%
% ```matlab
% 	p.benchmark()
% ```
%
% **Description**
%
% Benchmarks performance on current hardware
% and saves results to `~/.psych/benchmark.mat`.
%
% See Also:
% xgrid.printLog
% xgrid.showWorkerStates
% 

function benchmark(self)

original_dir = pwd;

corelib.assert(isempty(self.x),'benchmark cannot run if there a xolotl object already configured')

cd([fileparts(fileparts(which('xgrid'))) filesep 'tests'])

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.add('compartment','AB','A',0.0628,'vol',vol);
x.AB.add('bucholtz/CalciumMech','phi',phi);


x.AB.add('liu/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);
x.dt = .05;
x.t_end = 20e3;


% in this example, we are going to vary the maximal conductances of the Acurrent and the slow calcium conductance in a grid


parameters_to_vary = {'*.CaS.gbar','*.ACurrent.gbar'};

g_CaS_space = linspace(0,100,25);
g_A_space = linspace(100,300,25);

all_params = NaN(2,length(g_CaS_space)*length(g_A_space));
c = 1;
for i = 1:length(g_CaS_space)
	for j = 1:length(g_A_space)
		all_params(1,c) = g_CaS_space(i);
		all_params(2,c) = g_A_space(j);
		c = c + 1;
	end
end


self.cleanup;
self.n_batches = 2;
self.x = x;
self.batchify(all_params,parameters_to_vary);

% configure the simulation type, and the analysis functions
self.sim_func = @xgrid_test_func;


tic
self.simulate;
wait(self.workers)
t = toc;
disp(['Finished in ' mat2str(t) ' seconds. Total speed = ' mat2str((length(all_params)*x.t_end*1e-3)/t)])

speed = (size(all_params,2)*x.t_end*1e-3)/t;
seld.speed = speed;

save('~/.psych/benchmark.mat','speed')
