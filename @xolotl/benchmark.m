%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% benchmarks a model with varying dt
% 

function benchmark(self)

original_dt = self.sim_dt;
original_t_end = self.t_end;
original_state = self.closed_loop;

self.t_end = 5e3;
self.closed_loop = false;

max_dt = 1000; 
K = 1:max_dt;
all_dt = K(rem(max_dt,K) == 0);
all_dt = all_dt/1e3;

all_Q = NaN*all_dt;
all_speed = NaN*all_dt;


self.sim_dt = all_dt(1);
self.dt = max_dt*1e-3;
tic
V0 = self.integrate;
V0_diff = diff(V0);
all_speed(1) = toc;


for i = 2:length(all_dt)

	self.sim_dt = all_dt(i);
	tic
	V = self.integrate;
	all_speed(i) = toc;

	% measure distance b/w diff-embedded attractors
	V_diff = diff(V);

	this_cost = 0;
	for j = 2:10:length(V0_diff)
		for k = 1:size(V,2)
			this_cost = this_cost + min(sqrt((V0_diff(j,k) - V_diff(:,k)).^2 + (V0(2:end,k) - V(2:end,k)).^2));
		end
	end

	all_r2(i) = this_cost;

end


figure('outerposition',[0 0 1001 500],'PaperUnits','points','PaperSize',[1000 500]); hold on
subplot(1,2,1); hold on
yyaxis left
plot(all_dt,all_r2,'-+')
set(gca,'YScale','log')
xlabel('dt (ms)')
ylabel('Error (a.u.)')

yyaxis right
plot(all_dt,(self.t_end/1e3)./all_speed,'-o')
ylabel('Speed (X realtime)')
set(gca,'YScale','log','XScale','log')


% now vary t_end to see the overhead costs 
self.sim_dt = .1;
self.closed_loop = true;
all_t_end = round(logspace(1,6,20));

all_T = NaN*length(all_t_end);

for i = 1:length(all_t_end)
	self.t_end = all_t_end(i);
	tic;
	self.integrate;
	all_T(i) = toc;
end

all_T = all_T*1e3;

subplot(1,2,2); hold on
plot(all_t_end,all_t_end./all_T,'-o')
xlabel('Simulation time (ms)')
set(gca,'XScale','log','YScale','log','XTick',logspace(1,6,6))
ylabel('Speed (X realtime)')

prettyFig();



self.sim_dt = original_dt;
self.t_end = original_t_end;
self.closed_loop = original_state;