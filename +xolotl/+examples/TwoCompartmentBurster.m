% makes a realistic cable with some options
function x = TwoCompartmentBurster()



resistivity = 1e-3; % MOhm mm; 



r_soma = .025;
L_soma = .025; % mm, Jason's guesstimates
f = 14.96;

r_neurite = .01;
L_neurite = .35; % mm, from Otopalik et al

shell_thickness = .01; % 10 micrometres

x = xolotl;

x.add('compartment','CellBody','radius',r_soma,'len',L_soma,'Ca_out',3000,'tree_idx',0,'shell_thickness',shell_thickness);
x.CellBody.add('prinz/Calcium','f',f)


x.add('compartment','Axon','radius',r_neurite,'len',L_neurite,'Ca_out',3000);
x.Axon.add('prinz/Calcium','f',f)




channels = {'NaV','CaT','CaS','ACurrent','KCa','Kd','HCurrent'};
gbar = [1000        25  60     500         50  1000 .1];
E =         [50   30  30 -80 -80 -80   -20];

comp_names = x.Children;

% soma has everythign except NaV
x.CellBody.add('prinz/CaT','gbar',25);
x.CellBody.add('prinz/CaS','gbar',60);
x.CellBody.add('prinz/ACurrent','gbar',500);
x.CellBody.add('prinz/KCa','gbar',50);
x.CellBody.add('prinz/Kd','gbar',1e3);
x.CellBody.add('prinz/HCurrent','gbar',.1);
x.CellBody.add('Leak','gbar',.1)

% axon has NaV, Kd, Leak, ACurrent
x.Axon.add('prinz/NaV','gbar',1000);
x.Axon.add('prinz/Kd','gbar',1000);
x.Axon.add('prinz/ACurrent','gbar',500);
x.Axon.add('Leak','gbar',.1)

x.CellBody.tree_idx = 0;
x.connect('CellBody','Axon','Axial','resistivity',resistivity);
x.connect('Axon','CellBody','Axial','resistivity',resistivity);



x.dt = .1;
x.sim_dt = .05;
x.t_end = 5e3;
