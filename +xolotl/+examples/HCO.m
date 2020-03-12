function x = HCO()

channels = {'prinz/NaV','prinz/CaT','prinz/CaS','prinz/ACurrent','prinz/KCa','prinz/Kd','generic/HCurrent','Leak'};


x = xolotl;

x.add('compartment','C1');
x.C1.add('prinz/CalciumMech');

for i = 1:length(channels)
	x.C1.add(channels{i});
end

% configure gbars
x.C1.set('*gbar', [200,  40,   0,   .5,   0,  250,    0.3, 1000])
x.set('*Leak.E',-50)

x.add(copy(x.C1),'C2')


x.C1.HCurrent.tau = 5e3;
x.C2.HCurrent.tau = 5e3;

x.connect('C1','C2','prinz/Chol','gmax',10);
x.connect('C2','C1','prinz/Chol','gmax',10);

x.C1.V = -10;
x.C2.V = -60;

x.sim_dt = .1;
x.dt = .1;
x.t_end = 30e3;