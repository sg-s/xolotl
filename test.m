% test script for matlab wrapper 


x = xolotl;
x.addCompartment('AB',-70,0.05,10,0.0628,1.496,3000,0.05,200);
x.addConductance('AB','liu/NaV',1830,30);
x.addConductance('AB','liu/CaT',23,30);
x.addConductance('AB','liu/CaS',27,30);
x.addConductance('AB','liu/ACurrent',246,-80);
x.addConductance('AB','liu/KCa',980,-80);
x.addConductance('AB','liu/Kd',610,-80);
x.addConductance('AB','liu/HCurrent',10.1,-20);
x.addConductance('AB','Leak',.99,-50);

x.addCompartment('LP',-70,0.05,10,0.0628,14.96,3000,0.05,200);
x.addConductance('LP','liu/NaV',1830,30);
x.addConductance('LP','liu/CaT',23,30);
x.addConductance('LP','liu/CaS',27,30);
x.addConductance('LP','liu/ACurrent',246,-80);
x.addConductance('LP','liu/KCa',980,-80);
x.addConductance('LP','liu/Kd',610,-80);
x.addConductance('LP','liu/HCurrent',10.1,-20);
x.addConductance('LP','Leak',.99,-50);



x.compile;
x.dt = 50e-3;
x.t_end = 5000;
[V,Ca] = x.integrate;

figure, plot(V)