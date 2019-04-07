
%%
% In this demo, we switch from a deterministic model
% of channel and subunit kinetics to a stochastic model
% that we integrate using the Euler-Maruyama method
% using the approximate Langevin method 


x = xolotl.examples.HodgkinHuxley;
x.approx_channels = 0;

all_area = logspace(-7,-5,20);
N = 3;
all_f = zeros(length(all_area),N);

x.t_end = 10e3;
x.sim_dt = .05;
x.dt = .1;

x.stochastic_channels = 1;




for j = 1:N
	for i = 1:length(all_area)
		x.reset;
		x.AB.A = all_area(i);

		V = x.integrate;
		all_f(i,j) = xtools.findNSpikes(V,0)/10;
	end
end

figure('outerposition',[300 300 1200 901],'PaperUnits','points','PaperSize',[1200 901]); hold on

show_area = corelib.logrange(min(all_area)*5,max(all_area),4);
clear ax
for i = 1:4
	ax(i) = subplot(4,2,(i-1)*2+1); hold on
	x.reset;
	x.AB.A = show_area(i);

	V = x.integrate;
	time = (1:length(V))*1e-3*x.dt;
	plot(time,V,'k')
	set(gca,'YLim',[-80 50])
	th = title(['Area = ' strlib.oval(x.AB.A) 'mm^2']);
	th.FontWeight = 'normal';
	
end


axf = subplot(1,2,2); hold on
errorbar(all_area,mean(all_f,2),corelib.sem(all_f'),'k')
set(gca,'XScale','log','YScale','linear')
xlabel('Area (mm^2)')
ylabel('Firing rate (Hz)')

axf.Position = [.6 .3 .25 .4];

figlib.pretty()

axlib.makeEphys(ax)