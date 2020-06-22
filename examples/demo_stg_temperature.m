
%%
% This demo sets up a neuron with temperature sensitive channels
% and a calcium mechanism, and integrates it at two different
% temperatures to show how temperature can change the 
% behaviour of the neuron 

% set up a neuron using temperature-sensitive components.
% the normal components are not temperature sensitive 
x = xolotl.examples.neurons.BurstingNeuron('prefix','prinz-temperature','CalciumMech','buchholtz-temperature');


% try to download a binary
% so that people without compilers can use this too
try
	x.download
end


% set Q10s of all components 
RandStream.setGlobalStream(RandStream('mt19937ar','Seed',9)); 
x.set('*Q',2 + randn(19,1)/10)

x.sim_dt = .1;
x.dt = .1;
x.t_end = 5e3;
x.closed_loop = false;

% integrate
V = x.integrate;
time = (1:length(V))*1e-3*x.dt;

% increase temperature and integrate 
x.temperature = 22;
V2 = x.integrate;

figure('outerposition',[0 0 1000 500],'PaperUnits','points','PaperSize',[1000 500]); hold on

plot(time,V,'k')
plot(time,V2,'r')
xlabel('Time (s)')
ylabel('V_{m} (mV)')
legend({'11C','22C'})
set(gca,'XLim',[0 6],'YLim',[-80 50])
figlib.pretty('PlotLineWidth',1,'LineWidth',1)
drawnow

