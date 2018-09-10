% this tests generates and integrates
% a bursting neuron neuron that reproduces Fig 3
% in Tim O'Leary's 2013 paper


vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.add('compartment','AB','A',0.0628,'vol',NaN);
x.AB.add('CalciumMech2','phi',phi);


x.AB.add('liu/NaV','gbar',@() 115/x.AB.A,'E',30);
x.AB.add('liu/CaT','gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add('liu/CaS','gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add('liu/ACurrent','gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add('liu/KCa','gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add('liu/Kd','gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add('liu/HCurrent','gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);


% this should fail since vol is NaN
try
	x.integrate;
	error('Failed to catch an error where we set vol to NaN using CalciumMech2')
catch err
	if any(strfind(err.message,'CalciumMech2'))
		disp('Successfully caught error:')
		disp(err.message)
		x.AB.vol = x.AB.A;
	else
		error('Something went wrong, but not what was planned.')
	end

end

x.t_end = 1e4;
x.integrate;
x.t_end = 1e3;

x.plot;

return

x.benchmark;