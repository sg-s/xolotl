%% demo_hco
% this example generates a half-center oscillators
% with release and escape mechanisms
%based on the model of leech heartbeat interneurons
% and plots voltage traces

x = xolotl.examples.networks.LeechHeartbeat;

% ---------- release mechanism -----------

Vth = -40; % synaptic threhold (mV)

x.set('*.Vth',Vth);

x.t_end = 40e3; % msec
x.output_type = 1;
data = x.integrate;


figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
subplot(2,2,1), plot(x.time,data.HN3L.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
ylabel('Membrane potential, mV'); title('Release')
ylim([-65 5]);
set(gca, 'Fontsize', 14)

subplot(2,2,3), plot(x.time,data.HN3R.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
xlabel('Time, sec'); ylabel('Membrane potential, mV')
ylim([-65 5]);
set(gca, 'Fontsize', 14)



% --------- escape mechanism -------------

Vth = -55; % synaptic threhold (mV)

x.set('*.Vth',Vth);

data = x.integrate;

subplot(2,2,2), plot(x.time,data.HN3L.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
ylabel('Membrane potential, mV'); title('Escape')
ylim([-65 5]);
set(gca, 'Fontsize', 14)

subplot(2,2,4), plot(x.time,data.HN3R.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
xlabel('Time, sec'); ylabel('Membrane potential, mV')
ylim([-65 5]);
set(gca, 'Fontsize', 14)

