
%%
% In this demo, we switch from a deterministic model
% of channel and subunit kinetics to a stochastic model
% that we integrate using the Euler-Maruyama method
% using the approximate Langevin method


x = xolotl();

x.add('compartment','AB','A',1e-5)

x.AB.add('chow/NaV','gbar',1200);
x.AB.add('chow/Kd','gbar',360);
x.AB.add('Leak','gbar',3,'E',-54.4);


x.approx_channels = 0;

all_area = logspace(-6,log10(400e-6),20);
N = 3;
all_f = zeros(length(all_area),N);

x.t_end = 10e3;
x.sim_dt = .1;
x.dt = .1;

x.stochastic_channels = 1;




for j = 1:N
	for i = 1:length(all_area)
		x.reset;
		x.AB.A = all_area(i);
		x.integrate;
		V = x.integrate;
		all_f(i,j) = xtools.findNSpikes(V,0) * x.dt;
	end
end

figure('outerposition',[300 300 1200 901],'PaperUnits','points','PaperSize',[1200 901]); hold on

show_area = [1 10 50 100 200 400]*1e-6;
clear ax
for i = 6:-1:1
	ax(i) = subplot(6,2,(i-1)*2+1); hold on
	x.reset;
	x.AB.A = show_area(i);

	x.integrate;
	V = x.integrate;
	time = (1:length(V))*1e-3*x.dt;
	plot(time,V,'k')
	set(gca,'YLim',[-90 50],'XLim',[0 1])
	th = title([strlib.oval(x.AB.A*1e6) 'um^2']);
	th.FontWeight = 'normal';

	if i < 6
		axis off
	end

end


axf = subplot(2,2,2); hold on
errorbar(all_area*1e6,mean(all_f,2),std(all_f')./mean(all_f,2)','k')
set(gca,'XScale','linear','YScale','linear')
xlabel('Area (um^2)')
ylabel('Firing rate (Hz)')

% fit an exponential
cf = fit(all_area(:),mean(all_f,2),'exp1');
plot(all_area*1e6,cf(all_area),'r')

% also show ISI distribution for 100um^2 neuron
x.reset;
x.AB.A = 10e-6;
x.integrate;
x.t_end = 100e3;
V = x.integrate;
st = xtools.findNSpikeTimes(V,xtools.findNSpikes(V,0));
isis = diff(st);

axisi = subplot(2,2,4); hold on
[hy,hx] = histcounts(isis);
stairs(axisi,hx(2:end)+mean(diff(hx)),hy,'k')
xlabel('Interspike interval (ms)')
legend('Area = 100 um^2')

figlib.pretty('PlotBuffer', 0.1)

axlib.makeEphys(ax(end),'time_scale',.1,'voltage_position',-90)
