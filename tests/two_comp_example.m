g_axial = 50; % nS

% geometry of soma is a sphere
r = 0.0707;
A = 4*pi*r*r;
vol = (4/3)*pi*r*r*r;
f = 14.96; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
% make 2 neurons
x.add('ABSoma','compartment','Cm',10,'A',A,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

x.add('ABDendrite','compartment','Cm',10,'A',A/10,'vol',vol/10,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

x.add('LPSoma','compartment','Cm',10,'A',A,'vol',vol,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);

x.add('LPDendrite','compartment','Cm',10,'A',A/10,'vol',vol/10,'phi',phi,'Ca_out',3000,'Ca_in',0.05,'tau_Ca',tau_Ca);


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

x.ABSoma.NaV.gbar = 0;
x.LPSoma.NaV.gbar = 0;

x.slice('ABDendrite',5,g_axial);
x.slice('LPDendrite',5,g_axial);

x.connect('ABDendrite','ABSoma',g_axial);
x.connect('LPDendrite','LPSoma',g_axial);


x.dt = .1;
x.sim_dt = .1;
x.t_end = 5e3;

% mark the soma as the soma
x.LPSoma.tree_idx = 0;
x.ABSoma.tree_idx = 0;


compartments = x.find('compartment');
N = length(compartments);

V = x.integrate;

