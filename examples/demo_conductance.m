% in this test, first we construct a neuron using
% built-in conductances, and then create
% custom conductances using the conductance class
% and compare that the two neurons are identical

x = xolotl.examples.neurons.BurstingNeuron('prefix','liu');



channels = x.AB.find('conductance');

%       A  CaS  CaT H KCa Kd leak NaV
all_p = [3  3   3   1  4  4   0    3];
all_q = [1  1   1   0  0  0   0    1];
is_Ca = [0  1   1   0  0  0   0    0];

for i = 1:length(channels)

	if strcmp(channels{i},'Leak')
		continue
	end


	[m_inf, h_inf, tau_m, tau_h] =  xolotl.getGatingFunctions(x.AB.(channels{i}).cpp_class_path);

	temp = conductance;
	temp.m_inf = m_inf;
	temp.h_inf = h_inf;
	temp.tau_m = tau_m;
	temp.tau_h = tau_h;

	temp.default_E = x.AB.(channels{i}).E;

	temp.p = all_p(i);
	temp.q = all_q(i);
	temp.is_Ca = logical(is_Ca(i));

	temp.generateCPPFile(channels{i});


end

% now construct a new object using these channels

x2 = xolotl;
x2.add('compartment','AB','A',x.AB.A);
x2.AB.add('prinz/CalciumMech','f',1.496);

x2.AB.add('custom/NaV','gbar',@() 115/x.AB.A,'E',30);
x2.AB.add('custom/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x2.AB.add('custom/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x2.AB.add('custom/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x2.AB.add('custom/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x2.AB.add('custom/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x2.AB.add('custom/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x2.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

x.sim_dt = .1;
x.dt = .1;
x.reset;
x.approx_channels = 0;
V = x.integrate;

x2.t_end = x.t_end;
x2.sim_dt = .1;
x2.dt = .1;
x2.approx_channels = 0;
x2.reset;
V2 = x2.integrate;

figure, hold on
plot(V)
plot(V2)

figlib.pretty()

% corelib.assert(sum(V2 - V)==0,'Something went wrong')
