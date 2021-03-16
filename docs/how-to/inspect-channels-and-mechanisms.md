This document will describe how to inspect conductances and mechanisms
and read out anything from them. 


# Inspecting conductances

xolotl does not return the `m` and `h` (activation and 
inactivation) variables from channels by default. This decision was 
made because it is not often that this is needed, and omitting 
these variables makes the code run faster and saves memory. If you do
want to pull these values out, this is what you have to do.

## Inspecting activation variables in most conductances

If the channel you want to inspect follows the standard Hodgkin-Huxley
formalism (most channels in xolotl do), and its integration routines
use the built in [conductance::integrate](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance2.hpp) methods, then you can use the 
`channelProbe` mechanism to read out values. Like so:

```matlab
% create a neuron with some channels
x = xolotl.examples.neurons.BurstingNeuron;

% let's probe the NaV channel
x.AB.NaV.add('ChannelProbe')

% now read out the activation variables and plot them
[V,~,M] = x.integrate;

```

Plotting M and comparing to the voltage shows us the activation and inactivation variables for the sodium channel lines up nicely with the spikes. 

![](https://user-images.githubusercontent.com/6005346/101928009-18665600-3ba3-11eb-95e6-9c7606e5f42d.png)


## Inspecting activation variables in custom conductances

Custom conductances may not use the Hodgkin-Huxley formalism, may have
their own integration routines, and may contain their own internal variables. To read out internal variables from these channels, you have
to do two things. First, your custom conductance must respond to `getState` requests and return every variable of interest. Add something like this to your conductance:

```C++

double customChannel::getState(int idx) {
    if (idx == 1) {return variable_1;}  
    else if (idx == 2) {return variable_2;}
    else {return std::numeric_limits<double>::quiet_NaN();}
}

```

and modify a copy of `ChannelProbe` to read out these variables:

```C++
int ChannelProbe::getState(double *mech_states, int idx) {
    // read out custom variables
    mech_states[idx] = channel->getState(1);
    idx++;
    mech_states[idx] = channel->getState(2);
    idx++;
    return idx;
}

```

# Inspecting mechanisms

Inspecting mechanisms is similar to the process described earlier. 