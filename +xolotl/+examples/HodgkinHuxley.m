
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

x.add('compartment','AB','A',1e-5)

x.AB.add('chow/NaV','gbar',1200);
x.AB.add('chow/Kd','gbar',480);

x.integrate;