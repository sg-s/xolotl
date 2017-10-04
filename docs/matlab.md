# How to use xolotl from within MATLAB

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
              


