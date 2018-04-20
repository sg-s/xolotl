% tests a neuron that reproduces Fig 3 in Tim's paper

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;
Ca_target = 0; % used only when we add in homeostatic control 

x = xolotl;
x.add('AB','compartment','Cm',10,'A',0.0628,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca,'Ca_target',Ca_target);

% set up a relational parameter
x.AB.vol = @() x.AB.A;

x.AB.add('liu-approx/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu-approx/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu-approx/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

all_I_ext = linspace(-1,3,50);
x.t_end = 5e3;
x.sim_dt = .1;
x.dt = .1;

all_f = NaN*all_I_ext;

figure('outerposition',[100 100 500 500],'PaperUnits','points','PaperSize',[1000 500]); hold on
xh = plot(NaN,NaN,'k-o');
xh_rev = plot(NaN,NaN,'r-o');
xlabel('I_{ext} (nA)')
ylabel('Firing rate (Hz)')
set(gca,'YLim',[-1 150],'XLim',[min(all_I_ext) max(all_I_ext)])

prettyFig();


for i = 1:length(all_I_ext)
	V = x.integrate(all_I_ext(i));
	all_f(i) = length(computeOnsOffs(V>0))/(x.t_end*1e-3);
	xh.XData = all_I_ext;
	xh.YData = all_f;
	drawnow;
end

all_I_ext = fliplr(all_I_ext);
all_f = NaN*all_I_ext;
for i = 1:length(all_I_ext)
	V = x.integrate(all_I_ext(i));
	all_f(i) = length(computeOnsOffs(V>0))/(x.t_end*1e-3);
	xh_rev.XData = all_I_ext;
	xh_rev.YData = all_f;
	drawnow;
end
