
%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

## make_bursting_neuron

**Syntax**

```matlab
x = xolotl.make_bursting_neuron('liu');
x = xolotl.make_bursting_neuron('prinz');
```

**Description**

makes a single-compartment neuron that bursts, using
channels from Liu et al. or Prinz et al. 


%}


function x = make_bursting_neuron(prefix)

if nargin == 0
	prefix = 'liu';
end

vol = 0.0628; % this can be anything, doesn't matter
f = 1.496; % uM/nA
tau_Ca = 200;
F = 96485; % Faraday constant in SI units
phi = (2*f*F*vol)/tau_Ca;

x = xolotl;
x.add('compartment','AB','A',0.0628,'vol',.0628);
x.AB.add('bucholtz/CalciumMech','phi',phi);


x.AB.add([prefix '/NaV'],'gbar',@() 115/x.AB.A,'E',30);
x.AB.add([prefix '/CaT'],'gbar',@() 1.44/x.AB.A,'E',30);
x.AB.add([prefix '/CaS'],'gbar',@() 1.7/x.AB.A,'E',30);
x.AB.add([prefix '/ACurrent'],'gbar',@() 15.45/x.AB.A,'E',-80);
x.AB.add([prefix '/KCa'],'gbar',@() 61.54/x.AB.A,'E',-80);
x.AB.add([prefix '/Kd'],'gbar',@() 38.31/x.AB.A,'E',-80);
x.AB.add([prefix '/HCurrent'],'gbar',@() .6343/x.AB.A,'E',-20);
x.AB.add('Leak','gbar',@() 0.0622/x.AB.A,'E',-50);

x.t_end = 2e3;
x.integrate;