% small script that tests xgrid
% this simulates 100 different neurons 



x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');
x.dt = .1;
x.sim_dt = .1;
x.t_end = 10e3;


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



tic 
p.simulate;
wait(p.workers)
t = toc;

%% This will also work
% this is a more bare-bones of doing things
% tic
% parfor i = 1:11
% 	p.simulate_core(i,1);
% end
% t = toc;

all_gbar = linspace(0,2e3,10);

V = zeros(10e4,10);

parfor i = 1:10
	x.set('AB.NaV.gbar',all_gbar(i));
	V(:,i) = x.integrate;
end

[all_data,all_params,all_param_idx] = p.gather;


disp(['Finished in ' mat2str(t) ' seconds. Total speed = ' mat2str((length(all_params)*x.t_end*1e-3)/t)])

burst_periods = all_data{1};
n_spikes_per_burst = all_data{2};
sim_time = all_data{3};


% assemble the data into a matrix for display
BP_matrix = NaN(length(g_CaS_space),length(g_A_space));
NS_matrix = NaN(length(g_CaS_space),length(g_A_space));
for i = 1:length(all_params)
	xx = find(all_params(1,i) == g_CaS_space);
	y = find(all_params(2,i) == g_A_space);
	BP_matrix(xx,y) = burst_periods(i);
	NS_matrix(xx,y) = n_spikes_per_burst(i);
end
BP_matrix(BP_matrix<0) = NaN;
NS_matrix(NS_matrix<0) = 0;

figure('outerposition',[0 0 1100 500],'PaperUnits','points','PaperSize',[1100 500]); hold on
subplot(1,2,1)
h = heatmap(g_A_space,g_CaS_space,BP_matrix);
h.Colormap = parula;
h.MissingDataColor = [1 1 1];
ylabel('g_CaS')
xlabel('g_A')
title('Burst period (ms)')

subplot(1,2,2)
h = heatmap(g_A_space,g_CaS_space,NS_matrix);
h.Colormap = parula;
h.MissingDataColor = [1 1 1];
ylabel('g_CaS')
xlabel('g_A')
title('#spikes/burst')

figlib.pretty();


% clean up error files
allfiles = dir('*.error');
for i = 1:length(allfiles)
	delete(allfiles(i).name)
end