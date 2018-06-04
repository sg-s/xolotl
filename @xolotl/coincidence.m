% coincidence.m
% coincidence factor b/w 2 spike trains as defined in 
% Jolivet et al. 2008 J Neuro Methods

function C = coincidence(model_spiketimes, data_spiketimes, dt, Delta)

model_spiketimes = nonnans(model_spiketimes);
data_spiketimes = nonnans(data_spiketimes);

N_model = length(model_spiketimes);
N_data = length(data_spiketimes);

f = N_model/(max(model_spiketimes)*dt);

N = 1 - 1*f*Delta;

avg_N_coinc = 1*f*Delta*N_data;

N_coinc = 0;

for i = 1:N_model
	for j = 1:N_data
		if abs(model_spiketimes(i) - data_spiketimes(j)) < Delta
			N_coinc = N_coinc + 1;
		end
	end
end

C = (N_coinc - avg_N_coinc)/(N*(N_data + N_model));
C = C/2;