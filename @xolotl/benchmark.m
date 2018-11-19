%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

### benchmark

performs a quick benchmarking of a given `xolotl` model. `benchmark` first varies the simulation time step, and measures how quickly the model integrates. It then varies `t_end`, and measures how fast it integrates at a fixed `sim_dt`. Usage

```
x.benchmark;
```

%}

function benchmark(self)



original_dt = self.sim_dt;
original_t_end = self.t_end;
original_state = self.closed_loop;

self.t_end = 10e3;
self.integrate;

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
all_speed(1) = toc;

for j = size(V0,2):-1:1
	[M0(:,:,j), V_lim(:,j), dV_lim(:,j)] = xolotl.V2matrix(V0(:,j));
end


all_Q(1) = 0;

for i = 2:length(all_dt)

	self.sim_dt = all_dt(i);
	tic
	V = self.integrate;
	all_speed(i) = toc;

	this_q = 0;
	for j = 1:size(V0,2)
		M(:,:,j) = xolotl.V2matrix(V(:,j),V_lim(:,j),dV_lim(:,j));
		this_q =  this_q + xolotl.matrixCost(M(:,:,j),M0(:,:,j));
	end


	% measure quality using LeMasson matrices
	all_Q(i) = this_q;


end


figure('outerposition',[0 0 1001 500],'PaperUnits','points','PaperSize',[1000 500]); hold on
subplot(1,2,1); hold on
yyaxis left
plot(all_dt,all_Q,'-+')
set(gca,'YScale','log')
xlabel('dt (ms)')
ylabel('Error (a.u.)')

yyaxis right
plot(all_dt,(self.t_end/1e3)./all_speed,'-o')
ylabel('Speed (X realtime)')
set(gca,'YScale','log','XScale','log')

drawnow


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

try
	prettyFig();
catch
end



self.sim_dt = original_dt;
self.t_end = original_t_end;
self.closed_loop = original_state;