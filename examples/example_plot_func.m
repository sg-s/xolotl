function example_plot_func(x)


% let's figure out if we need to make the plot or not
if ~isfield(x.handles,'example_plot') || ~isvalid(x.handles.example_plot.main_fig')

	% make the figure
	example_plot.main_fig = figure('outerposition',[300 300 1200 700],'PaperUnits','points','PaperSize',[1200 700]); hold on


	% make the subplots
	example_plot.v_trace = subplot(2,1,1); hold on
	example_plot.gbars = subplot(2,3,4); hold on
	example_plot.burst_metrics = subplot(2,3,5); hold on
	example_plot.Ca_embedding = subplot(2,3,6); hold on

	% make the plots 
	example_plot.plots.V = plot(example_plot.v_trace,NaN,NaN,'k');
	example_plot.plots.Ca_embedding = plot(example_plot.Ca_embedding,NaN,NaN,'k');
	example_plot.plots.burst_metrics_all = plot(example_plot.burst_metrics,NaN,NaN,'.','Color',[.5 .5 .5]);
	example_plot.plots.burst_metrics_this = plot(example_plot.burst_metrics,NaN,NaN,'r+');

	

	% attach to puppeteer so that it can be closed automatically
	x.handles.puppeteer_object.attachFigure(example_plot.main_fig)


	% labels
	xlabel(example_plot.burst_metrics,'Duty Cycle')
	ylabel(example_plot.burst_metrics,'Burst period (ms)')
	example_plot.burst_metrics.XLim = [0 1];
	example_plot.burst_metrics.YLim = [0 3e3];

	xlabel(example_plot.Ca_embedding,'[Ca^2^+] (uM)')
	ylabel(example_plot.Ca_embedding,'d [Ca^2^+] /dt (uM/ms)')

	x.handles.example_plot = example_plot;
	

end

example_plot = x.handles.example_plot;

% make the plots
x.output_type = 0;
[V,Ca] = x.integrate;
time = (1:length(V))*1e-3*x.dt;
example_plot.plots.V.XData = time;
example_plot.plots.V.YData = V;

x.plotgbars(example_plot.gbars,'AB')

% get the burst metrics
metrics = xtools.V2metrics(V,'sampling_rate',1/x.dt);


example_plot.plots.burst_metrics_this.XData = metrics.duty_cycle_mean;
example_plot.plots.burst_metrics_this.YData = metrics.burst_period;

example_plot.plots.burst_metrics_all.XData = [example_plot.plots.burst_metrics_all.XData metrics.duty_cycle_mean];
example_plot.plots.burst_metrics_all.YData = [example_plot.plots.burst_metrics_all.YData metrics.burst_period];

Ca = Ca(:,1); Ca = Ca(:);
dCa = [NaN; diff(Ca)];
example_plot.plots.Ca_embedding.XData = Ca;
example_plot.plots.Ca_embedding.YData = dCa;