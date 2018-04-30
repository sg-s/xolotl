
% in this example, we create three neurons and integrate them together:
% AB is a multi-compartment neuron connected with Axial synpases
% SingleComp is a single compartment model
%
% all neurons have similar parameters for comparison. 

axial_resitivity = 1e-3; % MOhm mm; 

% geometry is a cylinder
r = 0.0707;
L = r;
f = 14.96; % uM/nA
phi = 900;

x = xolotl;
x.cleanup; x.skip_hash = true;
	% make 3 neurons
	x.add('ABSoma','compartment','radius',r,'len',L,'phi',phi,'Ca_out',3000,'Ca_in',0.05);
	x.add('ABDendrite','compartment','radius',r,'len',L,'phi',phi,'Ca_out',3000,'Ca_in',0.05);

	x.add('SingleComp','compartment','radius',r,'len',L,'phi',phi,'Ca_out',3000,'Ca_in',0.05);

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
	x.slice('ABDendrite',5,axial_resitivity);

	x.connect('ABDendrite','ABSoma','Axial','resistivity',axial_resitivity);
	x.connect('ABSoma','ABDendrite','Axial','resistivity',axial_resitivity);

	x.ABSoma.tree_idx = 0;

x.skip_hash = false; x.sha1hash;

% give the AB compartments some specific voltages
% so C++ debugging is easier 
x.ABSoma.V = -60;
x.ABDendrite.V = -50;
x.ABDendrite2.V = -40;
x.ABDendrite3.V = -30;
x.ABDendrite4.V = -20;
x.ABDendrite5.V = -10;

x.dt = .1;
x.sim_dt = .1;
x.t_end = 10;

return



% mark the soma as the soma
% x.LPSoma.tree_idx = 0;
x.ABSoma.tree_idx = 0;


compartments = x.find('compartment');
N = length(compartments);

V = x.integrate;

