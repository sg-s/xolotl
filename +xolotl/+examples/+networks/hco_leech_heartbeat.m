

%{
              _       _   _
   __  _____ | | ___ | |_| |
   \ \/ / _ \| |/ _ \| __| |
    >  < (_) | | (_) | |_| |
   /_/\_\___/|_|\___/ \__|_|

## half-center oscillator

**Syntax**

```matlab
x = xolotl.examples.networks.hco();
```

**Description**

makes a half-center oscillator model based on leech heartbeat interneurons
from Hill et al. 2001, Journal of Computational Neuroscience. 

Synapse is a graded synapse as in Sharp et al. 1996

(In the Hill et al. 2001 model there are two types of inhibitory synaptic transmission
between the model elemental oscillator neurons:
graded transmission, which is dependent on the influx
of presynaptic Ca2+ through low-threshold Ca2+
channels (Angstadt and Calabrese, 1991); and spikemediated
transmission, which is dependent on influx
of presynaptic Ca2+ through high-threshold Ca2+ channels
during a spike (Lu et al., 1997). Spike-mediated
inhibition was modeled as a postsynaptic conductance
that is triggered by presynaptic spikes.)

%}

function x = hco_leech_heartbeat()

channels = {'hill/CaF','hill/CaS','hill/HCurrent','hill/KACurrent','hill/KCurrent1','hill/KCurrent2','Leak','hill/NaP','hill/NaV'};

x = xolotl;

A=1; % unknown, set to 1 for simplicity
x.add('compartment','HN3R','A',A,'Cm',0.5) % right heart interneuron
x.add('compartment','HN3L','A',A,'Cm',0.5) % left heart interneuron

    for j = 1:length(channels)
        x.HN3R.add([channels{j}]);
        x.HN3L.add([channels{j}]);        
    end
        
% configure gbars
gCaF=5; gCaS=3.2; gH=4; gKA=80; gK1=100; gK2=80; gNaP=5; gNaV=200; gL=8; % nS
gbar=[gCaF, gCaS, gH, gKA, gK1, gK2, gL, gNaP, gNaV];
gbar = gbar/(A*1000); % convert into uS/mm^2
%gCaF=5; gCaS=3.2; gH=4; gKA=100; gK1=80; gK2=200; gNaP=5; gNaV=200; gL=8/(A*1000); % nS
x.HN3R.set('*gbar', gbar);
x.HN3L.set('*gbar', gbar);

% configure reversal potentials
ECaF=135; ECaS=135; EH=-21; EKA=-70; EK1=-70; EK2=-70; ENaP=45; ENaV=45; EL=-60; % mV
E=[ECaF, ECaS, EH, EKA, EK1, EK2, EL, ENaP, ENaV];
x.HN3R.set('*E',E);
x.HN3L.set('*E',E);


% add reciprocal inhibitory synapses

%Vth=-40; % synaptic threhold (mV) for release mechanism
Vth=-55; % synaptic threhold (mV) for escape mechanim

gsyn=5; %nS

x.connect('HN3R','HN3L','Graded','gmax',gsyn,'Vth',Vth,'Delta',2); 
x.connect('HN3L','HN3R','Graded','gmax',gsyn,'Vth',Vth,'Delta',2); 


x.HN3R.V = -50.1; x.HN3L.V = -50; % set initial conditions to be slightly different for two cells







