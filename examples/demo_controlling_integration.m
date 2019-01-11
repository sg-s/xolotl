%%
% In this example, we illustrate how we can control
% integration and request different forms of output
% and how this can impact memory use

x = xolotl.examples.BurstingNeuron;

g0 = 1e-1+1e-1*rand(8,1);
x.set('*gbar',g0);
x.AB.Leak.gbar = .099;

x.AB.Ca_target = 7;


x.AB.NaV.add('oleary/IntegralController','tau_m',666);
x.AB.CaT.add('oleary/IntegralController','tau_m',55555);
x.AB.CaS.add('oleary/IntegralController','tau_m',45454);
x.AB.ACurrent.add('oleary/IntegralController','tau_m',5000);
x.AB.KCa.add('oleary/IntegralController','tau_m',1250);
x.AB.Kd.add('oleary/IntegralController','tau_m',2000);
x.AB.HCurrent.add('oleary/IntegralController','tau_m',125000);

x.closed_loop = false;
x.t_end = 1e6;
x.dt = .1;
x.sim_dt = .1;


% normal output type, variables separated
x.output_type = 0;
[V,Ca] = x.integrate;

% measure memory footprint
temp = whos('V','Ca');
memory_usage(1) = sum([temp.bytes]);


figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
subplot(2,3,1)
mtools.neuro.raster(xtools.findNSpikeTimes(V,xtools.findNSpikes(V)),'Color','k')
set(gca,'XLim',[0 10],'YTick',[])
drawnow


subplot(2,3,2)
mtools.neuro.raster(xtools.findNSpikeTimes(V,xtools.findNSpikes(V)),'Color','k')
set(gca,'XLim',[1e3-1 1e3],'YTick',[])

% now use C++ spike finding
x.output_type = 2;
x.dt = 10;
data = x.integrate;

temp = whos('data');
memory_usage(2) = temp.bytes;

subplot(2,3,4)
mtools.neuro.raster(data.AB.spiketimes,'Color','r')
set(gca,'XLim',[0 10],'YTick',[])
xlabel('Time (s)')

subplot(2,3,5)
mtools.neuro.raster(data.AB.spiketimes,'Color','r')
set(gca,'XLim',[1e3-1 1e3],'YTick',[])
xlabel('Time (s)')

subplot(2,3,6); hold on
b = bar(1,memory_usage(1),'FaceColor','k');
b = bar(2,memory_usage(2),'FaceColor','r');
set(gca,'YScale','log','XTick',[1 2],'XLim',[0 3],'XTickLabel',{'Normal mode','spikes detected in C++'},'YLim',[1e7 1e9],'XTickLabelRotation',45)
ylabel('Memory used (bytes)')
drawnow