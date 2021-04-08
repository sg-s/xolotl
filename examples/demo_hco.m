%% demo_hco
% this example generates a half-center oscillator
% with release and escape mechanisms
%based on the model of leech heartbeat interneurons
% and plots voltage traces

x = xolotl.examples.networks.LeechHeartbeat;

% ---------- release mechanism -----------

Vth = -40; % synaptic threshold (mV)

x.set('*.Vth',Vth);

x.t_end = 40e3; % msec
x.output_type = 1;
data = x.integrate;


figure('outerposition',[300 300 1200 600],'PaperUnits','points','PaperSize',[1200 600]); hold on
subplot(2,2,1), plot(x.time,data.HN3L.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
ylabel('V_m (mV)'); title('Release')
ylim([-65 5]);

subplot(2,2,3), plot(x.time,data.HN3R.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
xlabel('Time (s)'); ylabel('V_m (mV)')
ylim([-65 5]);



% --------- escape mechanism -------------

Vth = -55; % synaptic threshold (mV)

x.set('*.Vth',Vth);

data = x.integrate;

subplot(2,2,2), plot(x.time,data.HN3L.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
ylabel('V_m (mV)'); title('Escape')
ylim([-65 5]);

subplot(2,2,4), plot(x.time,data.HN3R.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
xlabel('Time (s)'); ylabel('V_m (mV)')
ylim([-65 5]);

figlib.pretty()