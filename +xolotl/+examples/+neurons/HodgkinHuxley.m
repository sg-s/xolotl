
%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

## HodgkinHuxley

**Syntax**

```matlab
x = xolotl.examples.HodgkinHuxley('liu');
x = xolotl.examples.HodgkinHuxley('prinz');
```

**Description**

makes a single-compartment neuron that bursts, using
channels from Liu et al. or Prinz et al. 


%}


function x = HodgkinHuxley(varargin)

x = xolotl(varargin{:});

x.add('compartment','HH','A',1e-5)

x.HH.add('liu/NaV','gbar',1e3);
x.HH.add('liu/Kd','gbar',300);
x.HH.add('Leak', 'gbar', 1);

x.integrate;