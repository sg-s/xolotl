
%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

## BurstingNeuron

**Syntax**

```matlab
x = xolotl.examples.BurstingNeuron('liu');
x = xolotl.examples.BurstingNeuron('prinz');
```

**Description**

makes a single-compartment neuron that bursts, using
channels from Liu et al. or Prinz et al. 


%}


function x = HodgkinHuxley(varargin)

x = xolotl(varargin{:});

x.add('compartment','AB','A',1e-5)

x.AB.add('prinz/NaV','gbar',1200);
x.AB.add('hodgkin-huxley/Kd','gbar',480);

x.integrate;