function activation_func_plot_func(x)


% let's figure out if we need to make the plot or not
if ~isfield(x.handles,'AM') || ~isvalid(x.handles.AM.main_fig')

	% make the figure
	AM.main_fig = figure('outerposition',[300 300 1200 700],'PaperUnits','points','PaperSize',[1200 700],'Color','w'); hold on


	% make the subplots
	AM.v_trace = subplot(2,4,5:6); hold on
	xlabel(AM.v_trace,'Time (ms)')
	ylabel(AM.v_trace,'V_m (mV)')

	AM.I_trace = subplot(2,4,7:8); hold on
	xlabel(AM.I_trace,'Time (ms)')
	ylabel(AM.I_trace,'I (nA)')

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

	AM.plots.I_NaV = plot(AM.I_trace,NaN,NaN,'r');
	AM.plots.I_Kd = plot(AM.I_trace,NaN,NaN,'b');
	AM.plots.I_Leak = plot(AM.I_trace,NaN,NaN,'k');

	AM.plots.NaV_taum = plot(AM.tau,NaN,NaN,'LineWidth',2);
	AM.plots.NaV_tauh = plot(AM.tau,NaN,NaN,'LineWidth',2);
	AM.plots.Kd_taum = plot(AM.tau,NaN,NaN,'LineWidth',2);
	legend([AM.plots.NaV_taum,AM.plots.NaV_tauh,AM.plots.Kd_taum],{'NaV \tau_m','NaV \tau_h','Kd \tau_m'})



	% attach to puppeteer so that it can be closed automatically
	x.handles.puppeteer_object.attachFigure(AM.main_fig)


	% labels
	x.handles.AM = AM;

	[V,~,~,I] = x.integrate;
	time = (1:length(V))*x.dt;
	AM.I_trace.XLim = [min(time) max(time)];
	linkaxes([AM.I_trace AM.v_trace],'x')
	

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

tau_min = -Inf;
tau_max = Inf;


m_tau_A = x.AB.NaV.m_tau_A;
m_tau_B = x.AB.NaV.m_tau_B;
m_tau_V_half = x.AB.NaV.m_tau_V_half;
m_tau_V_slope = x.AB.NaV.m_tau_V_slope;
tau_m = @(V) m_tau_A + m_tau_B./(1+exp((V - m_tau_V_half)./m_tau_V_slope));
AM.plots.NaV_taum.XData = Vspace;
AM.plots.NaV_taum.YData = tau_m(Vspace);
tau_min = max([tau_min min(tau_m(Vspace))]);
tau_max = min([tau_max max(tau_m(Vspace))]);

m_tau_A = x.AB.Kd.m_tau_A;
m_tau_B = x.AB.Kd.m_tau_B;
m_tau_V_half = x.AB.Kd.m_tau_V_half;
m_tau_V_slope = x.AB.Kd.m_tau_V_slope;
tau_m = @(V) m_tau_A + m_tau_B./(1+exp((V - m_tau_V_half)./m_tau_V_slope));
AM.plots.Kd_taum.XData = Vspace;
AM.plots.Kd_taum.YData = tau_m(Vspace);
tau_min = min([tau_min min(tau_m(Vspace))]);
tau_max = max([tau_max max(tau_m(Vspace))]);

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
tau_min = min([tau_min min(AM.plots.NaV_tauh.YData)]);
tau_max = max([tau_max max(AM.plots.NaV_tauh.YData)]);


% switch the I_ext to a pulse
if length(x.I_ext) == 1
	I_ext = zeros(x.t_end/x.sim_dt,1);
	I_ext(1e3:2e3) = x.I_ext;
	x.I_ext = I_ext;
end

% integrate
[V,~,~,I] = x.integrate;
time = (1:length(V))*x.dt;



AM.plots.V.XData = time;
AM.plots.V.YData = V;

AM.plots.I_NaV.XData = time;
AM.plots.I_NaV.YData = I(:,3);

AM.plots.I_Kd.XData = time;
AM.plots.I_Kd.YData = I(:,1);

AM.plots.I_Leak.XData = time;
AM.plots.I_Leak.YData = I(:,2);

% center spike if it exists
idx = find(V>0,1,'first');

if any(idx)
	a = max([idx-100 1]);
	z = min([idx+300 length(V)]);
	AM.v_trace.XLim = [time(a) time(z)];
else
	AM.v_trace.XLim = [time(1) time(end)];
end

AM.v_trace.YLim = [min([-80 x.AB.Kd.E]) max([30 x.AB.NaV.E])];
m = min(I(:)); M = max(I(:));
AM.I_trace.YLim = [m - (M-m)/10 M + (M-m)/10];


AM.tau.YTick = 10.^(floor(log10(tau_min)):ceil(log10(tau_max)));
AM.tau_YLim = [10.^(floor(log10(tau_min))) 10.^(ceil(log10(tau_min)))];


figlib.pretty('PlotLineWidth',2);