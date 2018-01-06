%              _       _   _
%   __  _____ | | ___ | |_| |
%   \ \/ / _ \| |/ _ \| __| |
%    >  < (_) | | (_) | |_| |
%   /_/\_\___/|_|\___/ \__|_|
%
% benchmarks a model with varying dt
% 

function best_dt = benchmark(self)

original_dt = self.dt;
original_t_end = self.t_end;
original_state = self.closed_loop;

self.t_end = 5e3;
self.closed_loop = false;

all_dt = linspace(5e-3,1,50);
all_r2 = NaN*all_dt;
all_speed = NaN*all_dt;

self.skip_hash_check = true;


self.dt = all_dt(1);
tic
V = self.integrate;
all_speed(1) = toc;

for i = 1:size(V,2)
	spiketimes0{i} = computeOnsOffs(V(:,i) > 0);
end

time = self.dt:self.dt:self.t_end;
for i = 1:size(V,2)
	spiketimes0{i} = time(spiketimes0{i});
end

for i = 2:length(all_dt)

	self.dt = all_dt(i);
	tic
	V = self.integrate;
	all_speed(i) = toc;

	abort = false;

	for j = 1:size(V,2)
		spiketimes{j} = computeOnsOffs(V(:,j) > 0);
		if isempty(spiketimes{j})
			abort = true;
		end
	end

	if abort 
		continue
	end

	
	time = self.dt:self.dt:self.t_end;
	temp_r2 = NaN(size(V,2),1);
	for j = 1:size(V,2)
			spiketimes{j} = time(spiketimes{j});


		if length(spiketimes{j}) > length(spiketimes0{j})
			spiketimes{j} = spiketimes{j}(1:length(spiketimes0{j}));
		elseif length(spiketimes{j}) < length(spiketimes0{j})
			spiketimes{j} = [spiketimes{j} zeros(1,length(spiketimes0{j}) - length(spiketimes{j}))];
		end

		temp_r2(j) = rsquare(spiketimes0{j},spiketimes{j});


	end

	all_r2(i) = min(temp_r2);

end


if ~nargout
	figure('outerposition',[0 0 500 500],'PaperUnits','points','PaperSize',[1000 500]); hold on
	yyaxis left
	plot(all_dt,all_r2,'k+')
	set(gca,'YScale','log')
	xlabel('dt (ms)')
	ylabel('Quality (r^2)')
	set(gca,'YLim',[0 1])

	yyaxis right
	plot(all_dt,(self.t_end/1e3)./all_speed)
	ylabel('Speed (X realtime)')
	set(gca,'YScale','log')

	prettyFig();

else
	best_dt = all_dt(find(all_r2 < .99,1,'first'))/2;
end

self.dt = original_dt;
self.t_end = original_t_end;
self.closed_loop = original_state;