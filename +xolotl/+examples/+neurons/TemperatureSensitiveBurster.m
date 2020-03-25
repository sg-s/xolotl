function x = TemperatureSensitiveBurster()

x = xolotl;
x.add('compartment','AB','A',0.0628);

x.AB.add('prinz/CalciumMech');

x.AB.add('prinz-temperature/NaV','gbar',1000);
x.AB.add('prinz-temperature/CaT','gbar',25);
x.AB.add('prinz-temperature/CaS','gbar',60);
x.AB.add('prinz-temperature/ACurrent','gbar',500);
x.AB.add('prinz-temperature/KCa','gbar',50);
x.AB.add('prinz-temperature/Kd','gbar',1000);
x.AB.add('prinz-temperature/HCurrent','gbar',.1);