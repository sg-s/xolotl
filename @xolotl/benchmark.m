%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% benchmarks a model with varying dt
% 

function best_dt = benchmark(self)

original_dt = self.sim_dt;
original_t_end = self.t_end;
original_state = self.closed_loop;

self.t_end = 5e3;
self.closed_loop = false;

dt = 1000; 
K = 1:dt;
all_dt = K(rem(dt,K) == 0);
all_dt = all_dt/1e3;

all_r2 = NaN*all_dt;
all_speed = NaN*all_dt;

self.skip_hash_check = true;


self.sim_dt = all_dt(1);
self.dt = 1;
tic
V0 = self.integrate;
all_speed(1) = toc;



for i = 2:length(all_dt)

	self.sim_dt = all_dt(i);
	tic
	V = self.integrate;
	all_speed(i) = toc;


	this_r2 = NaN(size(V,2),1);
	for j = 1:size(V,2)
		this_r2(j) = rsquare(V(:,j),V0(:,j));
	end

	all_r2(i) = mean(this_r2);

end


if ~nargout
	figure('outerposition',[0 0 500 500],'PaperUnits','points','PaperSize',[1000 500]); hold on
	yyaxis left
	plot(all_dt,all_r2,'k-+')
	set(gca,'YScale','log')
	xlabel('dt (ms)')
	ylabel('Quality (r^2)')
	set(gca,'YLim',[0 1])

	yyaxis right
	plot(all_dt,(self.t_end/1e3)./all_speed,'-o')
	ylabel('Speed (X realtime)')
	set(gca,'YScale','log','XScale','log')

	prettyFig();

else
	best_dt = all_dt(find(all_r2 < .99,1,'first'))/2;
end

self.sim_dt = original_dt;
self.t_end = original_t_end;
self.closed_loop = original_state;