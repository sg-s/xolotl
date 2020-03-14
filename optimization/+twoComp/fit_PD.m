% this function fits a two-compartment xolotl model
% to metrics extracted from a real PD neuron
% this function will never halt, and keep finding 
% and saving models till manually terminated
% it's OK to manually terminate it -- all models
% that are considered good enough are saved

function fit()


data = twoComp.getData;

% set up the xfit object

p = twoComp.setup();


for i = 1:1e9
	disp(['Starting with random seed #' strlib.oval(i)])
	try

		% configure the data
		p.data = data(rem(i,length(data))+1);

		p.seed = rand(length(p.seed),1).*(p.ub - p.lb) + p.lb;
		for j = 1:n_epochs
			p.fit;
		end

		% save
		X = p.seed;
		x.set(p.parameter_names,X);
		this_cost = p.sim_func(x, p.data);

		if isnan(this_cost)
			continue
		end

		if make_plot
			figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
			x.integrate;
			V = x.integrate;
			time = (1:length(V))*x.dt*1e-3;
			plot(time,V(:,2),'k')
			set(gca,'XLim',[0 3])


		end

		if this_cost < 4


			disp(['Final Cost for this seed is ' strlib.oval(this_cost)])

			all_params(i,:) = X;
			all_cost(i) = this_cost;

			save(file_name,'all_params','all_cost')


		else
			disp('Cost too high, skipping...')
		end

	catch
		disp('Something went wrong. Ouch. ')
	end


end
