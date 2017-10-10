# How to use xolotl from within MATLAB

## Inputs

All xolotl objects have the same order of inputs. They are:

```
(1)     sim_params
(2-N+1) compartments
(N+2)   synapses
(N+3)   V_clamp
(N+4)   controllers
```

where `N` is the number of compartments. 

If you're not using an argument, for example, if you're simulating a network but not clamping it, you must explicitly skip that argument. `xolotl` does this for you automatically using `xolotl.integrate()`, but this is something to keep in mind if you run it using the compiled binary. 

## Outputs 

All `xolotl` objects return the same order of outputs. They are:

```
V
Ca
I_clamp
cond_states
syn_states
controller_states
```

Due to the order in which `xolotl` was developed, there are some non-obvious gotchas here:

* `V` is of size `[n_steps,N]` where `nsteps = t_end/dt` and `N` is the number of compartments 
* `Ca` is of size `[n_steps,2*N]`. The first `N` vectors are intracellular Calcium traces, and the next `N` are the reversal potential of Calcium as a function of time. 
* `cond_states` is a giant matrix with the `m` and `h` of every conductance in every compartment lumped together. The ordering here is determined by the ordering in `x.serialize`. Note that `cond_states` *does not* include `gbar`, even when you include `controllers` that change `gbar` 
*  `syn_states` is a `[n_steps,2*S]` matrix where `S` is the number of synapses. The `gbar` and activity `s` if each synapse is stored interleaved. 
* `controller_states` is a `[n_steps,2*C]` where `C` is the number of controllers. The `gbar` of the channels they control, and the mRNA level of the controller is stored interleaved. 


## Methods

To view all the methods of 	`xolotl`, type

```
methods(xolotl)
```

### `addCompartment`

Adds a compartment to your `xolotl` object. 

Usage: 

```
x.addCompartment('AB',V,Ca,C_m,A,vol,phi,Ca_ext,Ca_in,tau_Ca);
```

### `addConductance`   

Adds a conductances to an existing compartment 

### `addSynapse`           

Example:

```
x.addConductance('AB','prinz/NaV',gbar,E);
```
                
### `cleanup`     

Delete all `mexBridge` files and binaries 

### `compile`     

Compile `mexBridge` C++ file into a binary that MATLAB can call.

                                   
### `getChannelsInCompartment`

returns a cell array of channels in the compartment 

Usage

```
% get channels in first compartment 
these_chanels = getChannelsInCompartment(x_obj,1); 
```

### `getCompartmentNames`   

returns a cell array of compartment names.   
                      
### `hash`     

Hash the `xolotl` object. 

### `integrate`      

Integrate the ODEs, solve for Voltage and Calcium for all compartments. 

### `manipulate`      

Use puppeteer to change parameters on the fly while evaluating the model. 
    
                 
### `plot`       

Plot activation curves for a channel. 

### `transpile`     

Convert your `xolotl` pseudo-object into a mexable C++ file. 

### `updateLocalParameters`   

Given a cell array of structures with parameters, update the corresponding entries in your `xolotl` object.

### `viewCode`

View the automatically transpiled code in the `mexBridge.cpp` file that corresponds to your `xolotl` object.
              


