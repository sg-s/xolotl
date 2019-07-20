% tests speed per worker, while varying the number of workers, etc



vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.add('compartment','AB','A',0.0628,'vol',vol);
x.AB.add('CalciumMech2','phi',phi);


x.AB.add('liu/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);
x.dt = 50e-3;
x.t_end = 10e3;


% in this example, we are going to vary the maximal conductances of the Acurrent and the slow calcium conductance in a grid


parameters_to_vary = {'*.CaS.gbar','*.ACurrent.gbar'};

g_CaS_space = linspace(0,100,10);
g_A_space = linspace(100,300,10);

all_params = NaN(2,length(g_CaS_space)*length(g_A_space));
c = 1;
for i = 1:length(g_CaS_space)
	for j = 1:length(g_A_space)
		all_params(1,c) = g_CaS_space(i);
		all_params(2,c) = g_A_space(j);
		c = c + 1;
	end
end

if exist('p','var') && isa(p,'xgrid')
else
	p = xgrid();
end

p.cleanup;
p.n_batches = 1;
p.x = x;
p.batchify(all_params,parameters_to_vary);

% configure the simulation type, and the analysis functions 
p.sim_func = @xgrid_test_func;


num_cores = feature('numcores')*2;
speed_per_worker_mean = zeros(num_cores+1,1);
speed_per_worker_std = zeros(num_cores+1,1);
real_elapsed_time = NaN(num_cores+1,1);


% first do the foreground
p.simulate_core(1,1);
all_data = p.gather;
S = all_data{4};
speed_per_worker_mean(1) = mean((x.t_end*1e-3)./S);
speed_per_worker_std(1) = std((x.t_end*1e-3)./S);


figure, hold on
h = errorbar(0:num_cores,speed_per_worker_mean,speed_per_worker_std);
xlabel('# of workers in parallel')
ylabel('Speed/workers')
prettyFig;


% now do it varying the number of workers
for i = 2:length(speed_per_worker_mean)

	delete(gcp('nocreate'))

	parpool('local',i-1);

	p = xgrid;
	p.num_workers = i - 1;
	p.cleanup;
	p.n_batches = 1;
	p.x = x;
	p.batchify(all_params,parameters_to_vary);

	p.sim_func = @xgrid_test_func;

	tic

	p.simulate;
	wait(p.workers);

	real_elapsed_time(i) = toc;

	all_data = p.gather;
	S = all_data{4};

	speed_per_worker_mean(i) = mean((x.t_end*1e-3)./S);
	speed_per_worker_std(i) = std((x.t_end*1e-3)./S);

	h.YData = speed_per_worker_mean;
	h.YNegativeDelta = speed_per_worker_std/2;
	h.YPositiveDelta = speed_per_worker_std/2;

	drawnow

end
