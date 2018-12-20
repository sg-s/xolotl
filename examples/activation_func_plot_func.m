function activation_func_plot_func(x)


% let's figure out if we need to make the plot or not
if ~isfield(x.handles,'AM') || ~isvalid(x.handles.AM.main_fig')

	% make the figure
	AM.main_fig = figure('outerposition',[300 300 1200 700],'PaperUnits','points','PaperSize',[1200 700]); hold on


	% make the subplots
	AM.v_trace = subplot(2,3,4:5); hold on
	xlabel(AM.v_trace,'Time (ms)')
	ylabel(AM.v_trace,'V_m (mV)')

	AM.NaV = subplot(2,3,1); hold on
	xlabel(AM.NaV,'V_m (mV)')
	ylabel(AM.NaV,'m_{\infty}, h_{\infty}')
	title(AM.NaV,'NaV')

	AM.Kd = subplot(2,3,2); hold on
	xlabel(AM.Kd,'V_m (mV)')
	ylabel(AM.Kd,'m_{\infty}')
	title(AM.Kd,'Kd')

	AM.tau = subplot(2,3,3); hold on
	title(AM.tau,'Timescales')
	xlabel(AM.tau,'V_m (mV)')
	ylabel(AM.tau,'\tau (ms)')
	AM.tau.YScale = 'log';

	% make the plots 
	AM.plots.NaV_minf = plot(AM.NaV ,NaN,NaN,'k','LineWidth',2);
	AM.plots.NaV_hinf = plot(AM.NaV ,NaN,NaN,'r','LineWidth',2);

	AM.plots.Kd_minf = plot(AM.Kd ,NaN,NaN,'k','LineWidth',2);

	AM.plots.V = plot(AM.v_trace,NaN,NaN,'k');

	AM.plots.NaV_taum = plot(AM.tau,NaN,NaN,'LineWidth',2);
	AM.plots.NaV_tauh = plot(AM.tau,NaN,NaN,'LineWidth',2);
	AM.plots.Kd_taum = plot(AM.tau,NaN,NaN,'LineWidth',2);
	legend([AM.plots.NaV_taum,AM.plots.NaV_tauh,AM.plots.Kd_taum],{'NaV \tau_m','NaV \tau_h','Kd \tau_m'})



	% attach to puppeteer so that it can be closed automatically
	x.handles.puppeteer_object.attachFigure(AM.main_fig)


	% labels
	x.handles.AM = AM;
	

end

AM.v_trace.YLim = [x.AB.Kd.E x.AB.NaV.E];

AM = x.handles.AM;

% make the plots

%  NaV activation functions
Vspace = linspace(-80,50,1e3);

V_half = x.AB.NaV.m_V_half;
V_slope = x.AB.NaV.m_V_slope;
m_inf = @(V) 1./(1 + exp((V - V_half)./V_slope));
AM.plots.NaV_minf.XData = Vspace;
AM.plots.NaV_minf.YData = m_inf(Vspace);

V_half = x.AB.NaV.h_V_half;
V_slope = x.AB.NaV.h_V_slope;
h_inf = @(V) 1./(1 + exp((V - V_half)./V_slope));
AM.plots.NaV_hinf.XData = Vspace;
AM.plots.NaV_hinf.YData = h_inf(Vspace);


% Kd activation function
V_half = x.AB.Kd.m_V_half;
V_slope = x.AB.Kd.m_V_slope;
m_inf = @(V) 1./(1 + exp((V - V_half)./V_slope));
AM.plots.Kd_minf.XData = Vspace;
AM.plots.Kd_minf.YData = m_inf(Vspace);


% timescale plots
m_tau_A = x.AB.NaV.m_tau_A;
m_tau_B = x.AB.NaV.m_tau_B;
m_tau_V_half = x.AB.NaV.m_tau_V_half;
m_tau_V_slope = x.AB.NaV.m_tau_V_slope;
tau_m = @(V) m_tau_A + m_tau_B./(1+exp((V - m_tau_V_half)./m_tau_V_slope));
AM.plots.NaV_taum.XData = Vspace;
AM.plots.NaV_taum.YData = tau_m(Vspace);


m_tau_A = x.AB.Kd.m_tau_A;
m_tau_B = x.AB.Kd.m_tau_B;
m_tau_V_half = x.AB.Kd.m_tau_V_half;
m_tau_V_slope = x.AB.Kd.m_tau_V_slope;
tau_m = @(V) m_tau_A + m_tau_B./(1+exp((V - m_tau_V_half)./m_tau_V_slope));
AM.plots.Kd_taum.XData = Vspace;
AM.plots.Kd_taum.YData = tau_m(Vspace);

% NaV tau_h
h_tau_A = x.AB.NaV.h_tau_A1;
h_tau_B = x.AB.NaV.h_tau_B1;
h_tau_V_half = x.AB.NaV.h_tau_V_half1;
h_tau_V_slope = x.AB.NaV.h_tau_V_slope1;
tau_h1 = @(V) h_tau_A + h_tau_B./(1+exp((V - h_tau_V_half)./h_tau_V_slope));


h_tau_A = x.AB.NaV.h_tau_A2;
h_tau_B = x.AB.NaV.h_tau_B2;
h_tau_V_half = x.AB.NaV.h_tau_V_half2;
h_tau_V_slope = x.AB.NaV.h_tau_V_slope2;
tau_h2 = @(V) h_tau_A + h_tau_B./(1+exp((V - h_tau_V_half)./h_tau_V_slope));

AM.plots.NaV_tauh.XData = Vspace;
AM.plots.NaV_tauh.YData = tau_h1(Vspace).*tau_h2(Vspace);


% integrate
V = x.integrate;
time = (1:length(V))*x.dt;
AM.plots.V.XData = time;
AM.plots.V.YData = V;