g_axial = 100; % nS

% geometry of soma is a sphere
r = 0.0707;
A = 4*pi*r*r;
vol = (4/3)*pi*r*r*r;
f = 14.96; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.add('Soma','compartment','Cm',10,'A',A,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

x.add('Axon','compartment','Cm',10,'A',A/10,'vol',vol/10,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);
x.add('Dendrite','compartment','Cm',10,'A',A/10,'vol',vol/10,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);


prefix = 'prinz-approx/';
channels = {'ACurrent','CaS','CaT','HCurrent','KCa','Kd','NaV'};
g =           [469;      54;  12.4 ; 0.48;     52;  1250; 1962];
E =           [-80;      30;    30;  -20;     -80;   -80;  30 ];

compartments = x.find('compartment');
for i = 1:length(channels)
	for j = 1:length(compartments)
		x.(compartments{j}).add([prefix channels{i}],'gbar',g(i),'E',E(i));
	end
end

x.Soma.NaV.gbar = 0;
x.Dendrite.NaV.gbar = 0;

x.Axon.CaS.gbar = 0;
x.Axon.CaT.gbar = 0;

x.connect('Axon','Soma',g_axial);
x.connect('Dendrite','Soma',g_axial);

x.dt = .1;
x.sim_dt = .1;
x.t_end = 5e3;

% mark the soma as the soma
x.Soma.tree_idx = 0;

V = x.integrate;

N = length(compartments);

for i = 1:N
	subplot(N,1,i); hold on
	plot(V(:,i),'k')
	title(compartments{i})
end

