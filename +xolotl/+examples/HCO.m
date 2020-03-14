function x = HCO()

channels = {'prinz/NaV','prinz/CaT','prinz/CaS','prinz/ACurrent','prinz/KCa','prinz/Kd','generic/HCurrent','Leak'};


x = xolotl;

x.add('compartment','Cell1');
x.Cell1.add('prinz/CalciumMech');

for i = 1:length(channels)
	x.Cell1.add(channels{i});
end

% configure gbars
x.Cell1.set('*gbar', [200,  40,   0,   .5,   0,  250,    0.3, 1000])
x.set('*Leak.E',-50)

x.add(copy(x.Cell1),'Cell2')


x.Cell1.HCurrent.tau = 5e3;
x.Cell2.HCurrent.tau = 5e3;

x.connect('Cell1','Cell2','generic/Graded','gmax',10);
x.connect('Cell2','Cell1','generic/Graded','gmax',10);

x.Cell1.V = -10;
x.Cell2.V = -60;

x.sim_dt = .1;
x.dt = .1;
x.t_end = 30e3;