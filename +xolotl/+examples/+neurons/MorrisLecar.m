function x = MorrisLecar()


x = xolotl;
x.add('compartment','ML','A',1e-3,'Cm',2)
x.ML.add('morris-lecar/CaCurrent','gbar',4.4,'E',120);
x.ML.add('morris-lecar/Potassium','gbar',8);
x.ML.add('Leak','E',-60,'gbar',2);