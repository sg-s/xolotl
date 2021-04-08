%% demo_hco
% this example generates a half-center oscillators
% with release and escape mechanisms
%based on the model of leech heartbeat interneurons
% and plots voltage traces

x = xolotl.examples.networks.hco_leech_heartbeat;

% ---------- release mechanism -----------

Vth=-40; % synaptic threhold (mV)

x.set('HN3R.GradedHN3L.Vth',Vth); x.set('HN3L.GradedHN3R.Vth',Vth);

dt = .1; x.dt = dt; x.sim_dt = dt; x.t_end = 40e3; % msec
x.output_type=1;
[data] = x.integrate;

time = dt/10^3:dt/10^3:x.t_end/10^3;
clf,
subplot(2,2,1), plot(time,data.HN3L.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
ylabel('Membrane potential, mV'); title('Release')
ylim([-65 5]);
set(gca, 'Fontsize', 14)

subplot(2,2,3), plot(time,data.HN3R.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
xlabel('Time, sec'); ylabel('Membrane potential, mV')
ylim([-65 5]);
set(gca, 'Fontsize', 14)

%x.pref.show_Ca = false;
%x.plot

% --------- escape mechanism -------------

Vth=-55; % synaptic threhold (mV)

x.set('HN3R.GradedHN3L.Vth',Vth); x.set('HN3L.GradedHN3R.Vth',Vth);

[data] = x.integrate;

subplot(2,2,2), plot(time,data.HN3L.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
ylabel('Membrane potential, mV'); title('Escape')
ylim([-65 5]);
set(gca, 'Fontsize', 14)

subplot(2,2,4), plot(time,data.HN3R.V,'k','linewidth',1)
line([0 x.t_end/10^3],[Vth Vth],'color','k','linestyle','--')
xlabel('Time, sec'); ylabel('Membrane potential, mV')
ylim([-65 5]);
set(gca, 'Fontsize', 14)

%x.pref.show_Ca = false;
%x.plot
