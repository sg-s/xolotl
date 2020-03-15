

channels = {'prinz/NaV','prinz/CaT','prinz/CaS','prinz/ACurrent','prinz/KCa','prinz/Kd','generic/HCurrent','Leak'};


x = xolotl;

x.add('compartment','Int1');
x.Int1.add('prinz/CalciumMech');

for i = 1:length(channels)
	x.Int1.add(channels{i});
end

% configure gbars
x.Int1.set('*gbar', [1e3,  86,  6.7,   30,   10.6,  387, 1, 1875])
x.set('*Leak.E',-50)

x.add(copy(x.Int1),'LG')


x.Int1.HCurrent.tau = 2e3;
x.LG.HCurrent.tau = 2e3;

x.connect('Int1','LG','generic/Graded','gmax',100,'tau',425,'Vth',-38);
x.connect('LG','Int1','generic/Graded','gmax',100,'tau',425,'Vth',-38);

x.Int1.V = -50;
x.LG.V = -60;

x.t_end = 30e3;

temp = xolotl.examples.BurstingNeuron('prefix','prinz');
temp.AB.HCurrent.destroy;
temp.AB.add('generic/HCurrent','gbar',.1)
x.add(temp.AB,'AB');

x.connect('AB','Int1','generic/Graded','gmax',.5,'tau',100,'Vth',-35)