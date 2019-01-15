
% % first, voltage clamp a single-compartment neuron

% x = xolotl.examples.BurstingNeuron();
% x.set('*NaV*gbar',0)

% x.closed_loop = false;
% x.dt = .05;
% x.sim_dt = .05;

% V = x.integrate;
% V_clamp = sin((1:length(V))/1e3)*30 - 50;
% V_clamp = V_clamp + linspace(0,30,length(V_clamp));


% x.V_clamp = V_clamp(:);
% I = x.integrate;

% x.I_ext = I;
% V = x.integrate;

% figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
% plot(V_clamp,'k')
% plot(V,'r')

% prettyFig();

% if being_published
% 	snapnow
% 	delete(gcf)
% end

% return

% this demo shows how we can voltage clamp 
% a single compartment in a multi-compartment neuron
% model 

x = xolotl.examples.BurstingCable();
x.set('*HCurrent.gbar',0)
x.set('*NaV.gbar',0)
x.dt = .1;
x.sim_dt = .1;

x.closed_loop = false;




V = x.integrate;
time = (1:length(V))*x.dt*1e-3;


% create a sine wave clamp 
V_clamp = sin((1:length(V))/1e3)*30 - 50;
V_clamp = V_clamp + linspace(0,30,length(V_clamp));


% let's clamp the soma
x.V_clamp = V*NaN;
x.V_clamp(:,1) = V_clamp;

data = x.integrate;

I_ext = 0*data;
I_ext(:,1) = data(:,1);

x.I_ext = I_ext;

V = x.integrate;

figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
plot(V_clamp,'k')
plot(V(:,1),'r')



