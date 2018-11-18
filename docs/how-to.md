(see https://www.divio.com/blog/documentation/)

# How to:

# Set up compilers correctly 

## GNU/Linux

There are a couple of quirks specific to using the `MEX` compiler on Linux machines.

* `MATLAB` can be particular about the version of `g++` it works with. For best results, use the compiler version recommended by `MATLAB`. 
* In addition, it's best to point `MATLAB` towards the system compiler, rather than one installed through distributions like `Anaconda`. 
* On certain Linux distributions, `MEX` cannot identify the installed `g++` compiler, even when it exists on your path (e.g. `which g++` in the terminal works fine). The error looks something like this

```matlab
  >> !which g++
  /bin/g++
  >> !g++ -dumpversion
  8.1.0
  >> mex -setup C++
  Error using mex
  No supported compiler was found.
```
 
 `MATLAB` recommends changing your path so that you default to an older version of `g++`. This is not strictly necessary. `MATLAB` can still compile using `MEX` with newer versions of `g++` in most cases. Generally, downgrading to an older version of `g++` doesn't solve this problem. 

There is a relatively simply fix however. Credit goes to GitHub user [bonanza123](https://gist.github.com/bonanza123/) for figuring it out.

* First, download the proper version of `gcc/g++` as normal. If you use a package manager, there are generally legacy versions under `gcc-VERSION` where `VERSION` is the version number (e.g. 6). You can also find them [here](https://gcc.gnu.org/). 
* Second, change the `mex_LANG_glnxa64.xml` specification file, where `LANG` is either `C` or `C++`. This is typically found at `~/.matlab/R2018a/mex_C_glnxa64.xml` (where `R2018a` is the `MATLAB` version and we're looking for the `C` language file). 
* Replace all references to `$GCC` with the path to the soft-link to your `gcc` compiler (e.g. `/usr/bin/gcc-6`). If you don't have a soft-link to your compiler set up (i.e. `which gcc` doesn't tell you the path to the link), then you have to [set one up](https://askubuntu.com/questions/898578/how-can-i-change-which-gcc-directory). 
* Repeat this process for the `mex_C++_glnxa64.xml` file. It should be in the same location as the `C`-specific file. Sometimes `MATLAB` doesn't generate the `C++` `.xml` file, causing a lot of errors. If it doesn't exist, copy the `C`-specific version of the file, rename it to `mex_C++_glnxa64.xml` and replace all references to `gcc` with `g++` so that `MATLAB` is forced to use the correct compiler.

The problem is fixed if you see something like this in `MATLAB`:

```matlab
  >> mex.getCompilerConfigurations('C++')
  ans =

  CompilerConfiguration with properties:

             Name: 'g++'
     Manufacturer: 'GNU'
         Language: 'C++'
          Version: ''
         Location: '/usr/bin/g++-6'
        ShortName: 'g++'
         Priority: 'A'
          Details: [1×1 mex.CompilerConfigurationDetails]
       LinkerName: ''
    LinkerVersion: ''
           MexOpt: '/home/alec/.matlab/R2018a/mex_C++_glnxa64.xml'
```


# Construct models
The first step to constructing any model in `xolotl` is to initialize the object. In this section (and almost anywhere else), we will use the variable `x` to hold the `xolotl` object.

```matlab
% create a xolotl object
x = xolotl;
```

The `xolotl` object has many properties that govern overarching simulation and environment characteristics. You can access any of these properties using dot notation (e.g. `x.verbosity`).

### Flags
All flags unless otherwise specified and default to 0 or false. Generally, flags are logicals (false or true) unless they have to interact with the underlying `C++` code, in which case, they must be integers.

* The `verbosity` flag (0 to 99) toggles the amount of text printed to the console during compiling and simulation. It's handy for debugging. The verbosity flag goes up to 99. The higher the number, the more verbose the printed output.
* The `closed_loop` flag (false or true) determines whether initial conditions should be reset before a new simulation. If `closed_loop` is true, successive simulations will use the current state of the `xolotl` object (e.g. the end state of the previous simulation if you run `integrate` twice in a row). 
* The `approx_channels` flag (0 or 1) determines whether approximations to computing gating functions should be used. Look-up tables and approximations to the exponential function significantly increase computational speed, but decrease accuracy, especially at high temporal resolution in the data.
* The `solver_order` flag takes the values 0 or 4. In the 0 case, standard solvers are used (exponential Euler). In the 4 case, a Runge-Kutta 4th order method is used instead. This method is slower but more accurate.
* The `output_structure` flag (false or true) determines if outputs from the `integrate` function should be separate (false) or organized in a structure (true). The former is useful when you only want a few outputs or don't care about lots of variable names. The latter is useful when it's important to keep all the output data organized.

### Numerical Properties

* The `dt` value stores the fixed time step (default $50 \times 10^{-3}~\mathrm{ms}$) for outputs from simulation. Note that this is not the same as `sim_dt`. This value determines the number of time steps in the output vectors. If `dt` and `sim_dt` differ, the simulation vector is interpolated before being output -- useful for running ultra-high definition simulations but not saving all that data.
* In contrast, `sim_dt` is the actual fixed time step for the simulation (default $50 \times 10^{-3}~\mathrm{ms}$). 
* `t_end` is the simulation time (default $5 \times 10^{3}~\mathrm{ms}$). 

* The `temperature` property holds the *in-silico* preparation temperature (default 11 deg. C). This property only matters when using temperature-sensitive conductances or mechanisms.
* `temperature_ref` holds the "default" temperature so that $Q_{10}$ values can be used.
* The `I_ext` property stores the current to be injected as a scalar, vector, or matrix.
* The `V_clamp` property stores the voltage of clamped compartments as a matrix of `nSteps x nComps` where `nSteps` is the number of time-steps (`x.t_end * x.dt`) and `nComps` is the number of compartments in the `xolotl` object tree.

### Non-Numerical Properties
* The `manipulate_plot_func` property contains a cell of function handles which correspond to all plotting functions that are called when a property is changed while using the `manipulate` functionality. It defaults to the built-in `x.plot` function.
* The `pref.m` file contains editable preferences for your `xolotl` installation.

## The four types of components

Components are our term for the different building blocks of models (and thus `xolotl` objects). In general, all components are `cpplab` objects, meaning that they have properties that are useful and accessible in `C++` and `MATLAB`. 

Generating a component works in the same way, regardless of what it really is.

```matlab
component = cpplab('componentIdentifier', 'PropertyName', PropertyValue, ...)
```

The component identifier is very simply, a character vector that tells `cpplab` what kind of component to make. It is always the first argument when constructing a `cpplab` object.

### Compartments
Compartments represent three-dimensional sections of membrane. A neuron can consist of one or more compartments. In multi-compartment neurons, axial synapses with very fast kinetics connect compartments. Compartments in different neurons can be connected with chemical or electrical synapses.

You can create a compartment (independent of a `xolotl` object) using [cpplab](https://github.com/sg-s/cpplab).

```matlab
% create a compartment with a specified volume
AB = cpplab('compartment', 'vol', 0.01)
```
`AB` is a free-floating object. You can inspect it like you would any other `MATLAB` object.

```matlab
>>  AB

AB = 

compartment object with:

     hash : 44c3772
       Cm : 10
        A : 0.01
   radius : NaN
      vol : NaN
  Ca_average : NaN
  shell_thickness : NaN
  tree_idx : NaN
        V : -60
  neuron_idx : NaN
  Ca_target : NaN
       Ca : NaN
   Ca_out : 3000
      len : NaN
```

You can add it to your `xolotl` object.

```matlab
% add compartment AB to the xolotl object tree
x.add('AB', AB)
```

A handy shortcut for this is

```matlab
% add a compartment named AB with a specified volume
x.add('compartment', 'AB', 'vol', 0.01)
```

We will use this shortcut extensively in the documentation, but remember that you can always do things the "long" way.

There are many useful properties of each `compartment` object:
* The `hash` is an alphanumeric MD5 hash unique to the structure of this compartment.
* The specific membrane capacitance is `Cm` in $\mathrm{nF/mm^2}$.
* The membrane potential is `V` in $\mathrm{mV}$.

#### Size and shape of compartments
Compartments are assumed to be specified by their size or shape. In the case of size, compartments have a defined surface area `A` ($\mathrm{mm^2}$). If shape is instead defined, the compartment is assumed to be cylindrical with a `radius` and `len` (length). The surface area is then computed to be $2 \pi (r ^2 + rh)$ in single-compartment cases (and potentially lacking the cylindrical surface areas in the case of multi-compartment models). If the radius and length are defined (which they must be in multi-compartment cases), then the `A` property is ignored. Similarly, the compartment volume is contained within the `vol` property. In the case of a cylindrical compartment with defined radius and length, the volume is defined to be $\pi r^2 h$. 

#### Calcium properties
* `Ca` contains the intracellular calcium concentration in $\mu$M.
* `Ca_out` contains the extracellular calcium concentration in $\mu$M.
* Calcium is buffered very rapidly upon passing through the cell membrane. Many models compute intracellular calcium concentration with respect to the volume of an interior "shell." This value is stored in `shell_thickness` in mm.
* In homeostatic tuning models, it is sometimes important to keep track of an average intracellular calcium concentration, `Ca_average` in $\mu$M.
* In homeostatic tuning models, it is sometimes important to keep track of a target intracellular calcium concentration, `Ca_target` in $\mu$M.

### Conductances
A conductance object can be created using [cpplab](https://github.com/sg-s/cpplab).

```matlab
% instantiate the fast sodium conductance from Prinz et al. 2003
>> NaV = cpplab('prinz/NaV', 'gbar', 1000, 'E', 50)
NaV = 

 NaV object with:

          hash : a85ae87
          gbar : 1000
             E : 50
             m : NaN
             h : NaN
```
There are a few useful properties in each `conductance` object:
* The MD5 `hash` of this object.
* `gbar` contains the specific maximal conductance in $\mathrm{\mu S / mm^2}$.
* `E` contains the reversal potential in $\mathrm{mV}$.
* `m` contains the activation gating variable (unitless).
* `h` contains the (de)inactivation gating variable (unitless).

#### What conductances are available?
 All `C++` header files which define the conductances used in `xolotl` are contained in the `c++/conductances` folder in the `xolotl` directory. If you are unsure where this is, type this into your `MATLAB` prompt:

```matlab
fileparts(fileparts(which('xolotl')))
```

You can also list all conductances with

```matlab
cpplab.search('conductances')
```

### Synapses
Synapses connect compartments together. Synapses are stored in a vector of `cpplab` objects. Synapse objects contain the following properties:

* The MD5 `hash` of this object.
* `gbar` contains the maximal conductance in $\mathrm{\mu S}$.
* `s` contains the synaptic gating variable (unitless).

Synapses are created between two compartments using the `connect` function:

```matlab
% connect compartments AB and PD with an electrical synapse
x.connect('AB', 'PD')
% connect two compartments with an electrical synapse
% and specify properties
x.connect('AB', 'PD', 'gbar', 100)
% connect two compartments with a glutamatergic synapse
% kinetics from Prinz et al. 2004
x.connect('AB', 'LP', 'prinz/Glut')
% connect two compartments with a glutamatergic synapse
% and specify properties
x.connect('AB', 'LP', 'prinz/Glut', 'gbar', 100)
```

#### What synapses are available?

All `C++` header files which define the synapses used in `xolotl` are contained in the `c++/synapses` folder in the `xolotl` directory. If you are unsure where this is, type this into your `MATLAB` prompt:

```matlab
fileparts(fileparts(which('xolotl')))
```

You can also list all synapses with

```matlab
cpplab.search('synapses')
```


### Mechanisms 


## The `add` function and the object tree
The general-purpose `add` function adds [cpplab](https://github.com/sg-s/cpplab) objects to the `xolotl` tree. In brief, a `cpplab` object is a special type of `MATLAB` data structure that secretly  binds `MATLAB` code to `C++` code. The advantage of this is that the user gets to work entirely in `MATLAB` but simulations run with the full power and speed of `C++`. 

`xolotl` itself is a framework for creating and integrating neuronal and network models using `cpplab`.

### What can be added to what?
`compartments` are added directly to the `xolotl` object. `conductances` are added to `compartments`. `synapses` connect two compartments together but themselves are properties of the `xolotl` object. `mechanisms` are special types of `cpplab` objects that affect how `xolotl` performs computations. They include stochastic noise, calcium integration, homeostatic tuning, and so on. As such, they can be added to `compartments` or `conductances`.

### Exploring constructed models in the command line 
`xolotl` objects are *bona fide* `MATLAB` objects and can be readily manipulated in the command line.
Here is an example of a `xolotl` object, representing the structure of a stomatogastric ganglion model from crustacea published in Prinz *et al.* 2004. The code to produce this model can be found in `examples/demo_stg.m`.

```matlab
>> x

xolotl object with 
---------------------
+ AB  
  > ACurrent (g=500, E=-80)
  > CaS (g=60, E=26.173778091544)
  > CaT (g=25, E=26.173778091544)
  > HCurrent (g=0.1, E=-20)
  > KCa (g=50, E=-80)
  > Kd (g=1000, E=-80)
  > NaV (g=1000, E=50)
---------------------
+ LP  
  > ACurrent (g=200, E=-80)
  > CaS (g=40, E=76.6764125879106)
  > CaT (g=0, E=76.6764125879106)
  > HCurrent (g=0.5, E=-20)
  > KCa (g=0, E=-80)
  > Kd (g=250, E=-80)
  > Leak (g=0.3, E=-50)
  > NaV (g=1000, E=50)
---------------------
+ PY  
  > ACurrent (g=500, E=-80)
  > CaS (g=20, E=62.3932795257928)
  > CaT (g=24, E=62.3932795257928)
  > HCurrent (g=0.5, E=-20)
  > KCa (g=0, E=-80)
  > Kd (g=1250, E=-80)
  > Leak (g=0.1, E=-50)
  > NaV (g=1000, E=50)
---------------------
```
Displaying the `xolotl` object shows select properties of each conductance within each compartment. We can see that there are three compartments each with 7-8 conductances with varied maximal conductances. In addition, we can infer that the sodium channels have reversal potentials of $50~\mathrm{mV}$ and the potassium channels of $-80~\mathrm{mV}$. 

In `MATLAB`, all compartments and conductances in this view are hyperlinked and can be clicked on to view that particular object. We can also access this view through the command line. Let's look at the first compartment.

```matlab
>> x.AB

ans = 

 compartment object with:

             hash : bba3847
               Cm : 10
                A : 0.0628
  shell_thickness : NaN
         tree_idx : NaN
              vol : NaN
                V : -48.7425773412141
        Ca_target : NaN
       Ca_average : 97.5752632857105
       neuron_idx : NaN
              len : NaN
           radius : NaN
               Ca : 354.000031181001
           Ca_out : 3000
              NaV : NaV object
         HCurrent : HCurrent object
     CalciumMech1 : CalciumMech1 object
              CaS : CaS object
              KCa : KCa object
         ACurrent : ACurrent object
               Kd : Kd object
              CaT : CaT object
```

We see all the properties of a standard `compartment` object plus each of the `conductance`s added to the `xolotl` tree (using the `add` function). We can look closer at a conductance.

```matlab
>> x.AB.NaV

ans = 

 NaV object with:

          hash : a85ae87
          gbar : 1000
             m : 0.0107532302761698
             h : 0.64304369213413
             E : 50
```

Since these are `MATLAB` objects, we can edit the properties *in-situ*.

```matlab
% set AB's NaV maximal conductance to zero
>> x.AB.NaV.gbar = 0

xolotl object with 
---------------------
+ AB  
  > ACurrent (g=500, E=-80)
  > CaS (g=60, E=26.173778091544)
  > CaT (g=25, E=26.173778091544)
  > HCurrent (g=0.1, E=-20)
  > KCa (g=50, E=-80)
  > Kd (g=1000, E=-80)
  > NaV (g=0, E=50)
---------------------
+ LP  
  > ACurrent (g=200, E=-80)
  > CaS (g=40, E=76.6764125879106)
  > CaT (g=0, E=76.6764125879106)
  > HCurrent (g=0.5, E=-20)
  > KCa (g=0, E=-80)
  > Kd (g=250, E=-80)
  > Leak (g=0.3, E=-50)
  > NaV (g=1000, E=50)
---------------------
+ PY  
  > ACurrent (g=500, E=-80)
  > CaS (g=20, E=62.3932795257928)
  > CaT (g=24, E=62.3932795257928)
  > HCurrent (g=0.5, E=-20)
  > KCa (g=0, E=-80)
  > Kd (g=1250, E=-80)
  > Leak (g=0.1, E=-50)
  > NaV (g=1000, E=50)
---------------------
```

## Discovering components

`xolotl` has a robust library of model components and it is still growing! All components are defined by `C++` header files (`.hpp`) in the `xolotl/c++/` folder. Each type of component then has its own subfolder (e.g. `xolotl/c++/conductances/`). Within these folders components are contained within a folder by first author name. If the first author has been lucky enough to publish multiple papers with component equations (and feature in the `xolotl` component database!), the name is appended with the last two digits of the calendar year (e.g. `nadim98` and `nadim99`).

For example, the delayed rectifier potassium conductance published in Liu *et al.* 1998 can be found in `../xolotl/c++/conductances/liu/Kd.hpp`. 

### Using `cpplab.search()`

The `cpplab` system that underlies `xolotl` has a helpful method for finding components. Searching for an author or component name will return a cell array of character vectors containing all the possibilities. 

If all results are of the same component type (e.g. `conductances`), the path from that component type's folder is given. Otherwise, the full path is returned.

> You can find out where `xolotl` is on your computer is with `which('xolotl')`

Search for components by Astrid Prinz.

```matlab
>> a = cpplab.search('prinz')

a =

  16×1 cell array

    {'/home/alec/code/xolotl/c++/conductances/prinz/ACurrent.hpp'            }
    {'/home/alec/code/xolotl/c++/conductances/prinz/CaS.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz/CaT.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz/HCurrent.hpp'            }
    {'/home/alec/code/xolotl/c++/conductances/prinz/KCa.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz/Kd.hpp'                  }
    {'/home/alec/code/xolotl/c++/conductances/prinz/NaV.hpp'                 }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/ACurrent.hpp'}
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/CaS.hpp'     }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/CaT.hpp'     }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/HCurrent.hpp'}
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/KCa.hpp'     }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/Kd.hpp'      }
    {'/home/alec/code/xolotl/c++/conductances/prinz-temperature/NaV.hpp'     }
    {'/home/alec/code/xolotl/c++/synapses/prinz/Cholinergic.hpp'             }
    {'/home/alec/code/xolotl/c++/synapses/prinz/Glutamatergic.hpp'           }
```

Search for all fast, voltage-gated sodium channels.

```matlab
>> a = cpplab.search('NaV')

a =

  17×1 cell array

    {'custom/NaV.hpp'           }
    {'dethier/NaV.hpp'          }
    {'kispersky/NaV.hpp'        }
    {'liu/NaV.hpp'              }
    {'liu-euler/NaV.hpp'        }
    {'liu-temperature/NaV.hpp'  }
    {'nadim98/int1/NaV.hpp'     }
    {'nadim98/lg/NaV.hpp'       }
    {'nadim98/mcn1/NaV.hpp'     }
    {'odowd-aldrich/DmNaV.hpp'  }
    {'prinz/NaV.hpp'            }
    {'prinz-temperature/NaV.hpp'}
    {'rodriguez/Int1/NaV.hpp'   }
    {'soto-trevino05/NaV.hpp'   }
    {'traub/NaV.hpp'            }
    {'turrigiano/NaV.hpp'       }
    {'wicher/DmNaV.hpp'         }

```

# Save and load models

Simply use `save` and `load`, since `xolotl` objects are *bona fide* MATLAB objects. 
```matlab
% save a model as a .mat file
save('myModelFile.mat', 'x');
% load a xolotl object from a .mat file
load('myModelFile.mat');
```

# Modify existing models

## Viewing Parameters

### Using the `find` and `get` methods

The `find` and `get` functions are robust searching tools for accessing parameters in `xolotl`. They are especially useful when you want to change many parameters at once.

Both functions accept cells of character vectors or character vectors as arguments. The `find` function returns a list of all fields of the `xolotl` structure as a cell of character vectors that fit that description. The `get` function works the same way, except that it returns the actual values of those fields. Under the hood, the `get` function is really just an extension of `find`.

It is important to note that both functions will find all fields that match any part of the search query. You can use asterisks `*` to "wildcard" that is, mean "anything here." This is useful especially between parts you want to specifically match for (such as `'AB*gbar'`).

These functions also work "down the tree," meaning that they are functions of each compartment, conductance, and so on, and search only within the part of the tree specified. For example, `x.AB.find('*')` will fetch every property of the `AB` compartment, but none in other compartments.

Get all conductances in all compartments.
```matlab
>> x.find('cond') % x.find('condu'), x.find('conduct'), ... x.find('conductance') also work too

ans =

  8×1 cell array

    {'AB.ACurrent'}
    {'AB.CaS'     }
    {'AB.CaT'     }
    {'AB.HCurrent'}
    {'AB.KCa'     }
    {'AB.Kd'      }
    {'AB.Leak'    }
    {'AB.NaV'     }
```

Get the names of all conductances in the `AB` compartment.

```matlab
>> x.AB.find('conductance')

ans =

  8×1 cell array

    {'ACurrent'}
    {'CaS'     }
    {'CaT'     }
    {'HCurrent'}
    {'KCa'     }
    {'Kd'      }
    {'Leak'    }
    {'NaV'     }
```

Get the names of all maximal conductances in in `AB` compartment.

```matlab
>> x.find('AB*gbar')

ans =

  8×1 cell array

    {'AB.ACurrent.gbar'}
    {'AB.CaS.gbar'     }
    {'AB.CaT.gbar'     }
    {'AB.HCurrent.gbar'}
    {'AB.KCa.gbar'     }
    {'AB.Kd.gbar'      }
    {'AB.Leak.gbar'    }
    {'AB.NaV.gbar'     }
```

Get the names of all maximal conductances in the `AB` compartment from within the compartment's scope (note the importance of the wildcard `*`).

```matlab
>> x.AB.find('*gbar')

ans =

  8×1 cell array

    {'ACurrent.gbar'}
    {'CaS.gbar'     }
    {'CaT.gbar'     }
    {'HCurrent.gbar'}
    {'KCa.gbar'     }
    {'Kd.gbar'      }
    {'Leak.gbar'    }
    {'NaV.gbar'     }
```

Get the values of some arbitrary properties.

```matlab
>> x.get('AB*Current.E')

ans =

   -80
   -20
```

And some more, using a cell of character vectors as the function argument.

```matlab
>> x.AB.NaV.get({'E', 'gbar'})

ans =

   30.0000
    0.1815
```

## Changing parameters

### Using `x.set()`

## Saving snapshots
To quickly save and revert back to a certain set of numerical parameters in a model, you can use the `snapshot` and `reset` methods.

```matlab
% save a model's parameters as 'initial'
x.snapshot('initial')
% do something else to the model
x.I_ext = 0.2;
V = x.integrate;
% save this as a different state to return to later
x.snapshot('added_current');
% return to the previously saved 'initial' state
x.reset('initial');
```
## Hashing a model
Each `xolotl` object is hashed by the MD5 algorithm to determine a unit alphanumeric identifier. Two `xolotl` objects are different only if their underlying structure (compartments, types of conductances, synapses) are different -- not if parameters are different.

```matlab
% hash a xolotl object
>> x

xolotl object with 
---------------------
+ AB  
  > ACurrent (g=5, E=-80)
  > CaS (g=0.113, E=30)
  > CaT (g=0.191, E=30)
  > HCurrent (g=0.128, E=-20)
  > KCa (g=0.163, E=-80)
  > Kd (g=0.11, E=-80)
  > Leak (g=0.099, E=-50)
  > NaV (g=0.181, E=30)
---------------------
>> x.hash

ans =

    '6b3a55344dbd63074cfd703061f9ae4d'
>> % the hash doesn't change when you change the parameters

>> x.AB.ACurrent.gbar = 0;

>> x.hash

ans =  
    '6b3a55344dbd63074cfd703061f9ae4d'
```

# Running simulations 
`xolotl` is designed to solve for state variables of [conductance-based](http://www.scholarpedia.org/article/Conductance-based_models) neuronal and network models.
The voltage across the membrane $V$ is given by the conservation of current equation.

$$C_m \frac{dV}{dt} = -\sum_i I_i$$

where $C_m$ is the specific membrane capacitance and $I_i$ is a specific transmembrane current. Each current takes the form

$$I_i = g_i(V) (V - E_i)$$

where $g_i(V)$ is the instantaneous conductance and $E_i$ the ionic reversal potential. In many models, the conductance $g_i(V)$ is given by

$$g_i(V) = \bar{g}_i m_i^{p_i} h_i^{q_i}$$

where $\bar{g}_i$ is the maximal conductance in Siemens per unit area and $m$ and $h$ are gating variables $\in[0, 1]$. The gating variables themselves are defined by differential equations which depend on the membrane potential. These equations are nonlinear and usually quite stiff. For these reasons, numerical simulations are required to adequately solve them.

## Changing the time step and duration

In the case where the simulation time step (`sim_dt`) and the output time step (`dt`) are identical, the `dt` property only needs to be set.

```matlab
% set the simulation time to 10 seconds
x.t_end 	= 10e3; % ms
% set the time-step to 1 microsecond
x.dt 		= 1e-3; % ms
```

The `sim_dt` property determines the number of time-steps actually computed. The `dt` property determines the number of time-steps output. In the following example, the computer would perform 1 million (`x.t_end/x.sim_dt`) iterations but the output vector (or matrix) `V` is only `1000 x nComps` where `nComps` is the number of compartments in the `xolotl` object tree.

```matlab
% perform 1e6 iterations, interpolate at a ratio of 1/1000
x.t_end 	= 1000; % 1000 milliseconds
x.sim_dt	= 1e-3; % 0.001 milliseconds
x.dt 		= 1;	% 1 millisecond
V 			= x.integrate;
```

## Closed loop vs. open loop
The `closed_loop` flag (false or true) determines whether initial conditions should be reset before a new simulation. If `closed_loop` is true, successive simulations will use the current state of the `xolotl` object (e.g. the end state of the previous simulation if you run `integrate` twice in a row). 

```matlab
% use current state of model as initial conditions
x.closed_loop = true
V = x.integrate;
```

## The outputs of `x.integrate`

### (1) Voltage or Injected current
When the `x.V_clamp` property is not set, the first output of `x.integrate` is the voltage trace in the form of a `nSteps x nComps` matrix where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. The number of time steps in the output is determined by the simulation time and the output time step `x.t_end` and `x.dt`.

### (2) Calcium
The calcium trace is in the form of a `nSteps x 2*nComps` matrix where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. The first `nComps` columns are the intracellular calcium concentration (in $\mu$M) for each compartment in the serialized `xolotl` object tree. The next set of `nComps` columns are the calcium reversal potential (in mV).

### (3) Currents

### (4) Mechanism variables

## Inject current into compartments
Injected current is mediated by the `I_ext` property of the `xolotl` object.

If `I_ext` is a scalar, that amount of current in $\mathrm{nA}$ is injected into every compartment at all time.

```matlab
% inject 0.2 nA into each compartment
x.I_ext = 0.2;
```

If `I_ext` is a vector the length of the number of compartments, constant current will be added to each compartment in order (based on the serialized `xolotl` object).

```matlab
% add current to only the first compartment
x.I_ext = [0.2, 0];
```

If `I_ext` is a matrix, it should be of size `nSteps x nComps` where `nSteps` is the number of time steps and `nComps` is the number of compartments in the model. Current is added at each time step to each compartment in order (based on the serialized `xolotl` object).

```matlab
% add a variable current into one of two compartments
nSteps 		= x.t_end / x.dt;
I_ext		= zeros(nSteps, 2);
I_ext(:, 1) = 0.2 * rand(nSteps, 1);
x.I_ext 	= I_ext;
```

## Voltage clamping compartments

## Switching solvers

```matlab
x.solver_order = 4; % uses Runge Kutta 4
x.solver_order = 0; % default, uses exponential Euler
```

### The exponential Euler method
The exponential Euler method is a time-discrete solution to differential equations of the form:

$$\tau_V \frac{dV}{dt} = V_\infty - V$$

where $V = V(V, t)$ is the state variable and $\tau_V$ and $V_\infty$ are functions of $V$. This equation can be solved as follows:

Rearrange equation
$$\tau_V \frac{dV}{dt} = -(V + V_\infty)$$

Divide by $V - V_\infty$, multiply by $\tau_V$
$$ \frac{dV}{V-V_\infty} = -\frac{dt}{\tau_V}$$

Integrate, using the relation $\ln(x) = \int \frac{1}{x}dx$
$$\ln(V-V_\infty) = -\frac{t}{\tau_V} + \mathrm{const.}$$

Given a constant term $V(t_0)$, the equation can be written
$$V(t) = V_\infty + (V(t_0) - V_\infty) \exp \Big(-\frac{t-t_0}{\tau_V}\Big)$$

For a time step $\Delta t$, the voltage $V$ at time $t + \Delta t$ can be approximated from the voltage at time $t$, where $V_\infty = V_\infty \big(V(t)\big)$ and $\tau_V = \tau_V\big(V(t)\big)$
$$V(t + \Delta t) = V_\infty + (V(t) - V_\infty) \exp \Big(-\frac{\Delta t}{\tau_V}\Big)$$

This approximation is more accurate than a first order Euler method approximation, and is significantly faster than higher order (*viz.* Runge-Kutta) methods.
> See Ch. 5, Dayan, P. and Abbott, LF (2001) Theoretical Neuroscience, MIT Press for more information

### The Runge-Kutta fourth-order method

The [Runge-Kutta methods](http://lpsa.swarthmore.edu/NumInt/NumIntFourth.html) are extensions of forward Euler to higher derivative orders. Given a differential equation in the form

$$\frac{dV}{dt} = V'(t) = f(V, t)$$

with some initial condition $V(t_0) = V_0$. The first order (Euler) approximation for $V(t + \Delta t)$ given $V(t)$ is

$$V(t + \Delta t) = V(t) + f(V, t) \Delta t$$

Euler's method is accurate $\propto \Delta t$ per step. The Runge-Kutta fourth-order method uses four coefficients $k_1, k_2, ...$ to extend this method to accuracy of $\propto (\Delta t) ^4$ per step at the cost of speed. The coefficients are, for $V = V(t)$,

$$ k_1 = f(V, t)$$
$$k_2 = f\Big(V + k_1 \frac{\Delta t}{2}, ~t  + \frac{\Delta t}{2}\Big)$$
$$k_3 = f\Big(V + k_2 \frac{\Delta t}{2}, ~t  + \frac{\Delta t}{2}\Big)$$
$$k_4 = f\Big(V + k_3 \Delta t, ~t  + \Delta t \Big)$$

The time-evolution formula for the Runge-Kutta fourth order method is

$$V(t + \Delta t) = V(t) + \frac{k_1 + 2k_2 + 2k_3 + k_4}{6} \Delta t$$

The method is more accurate because slope approximations at fractions of $\Delta t$ are being taken and averaged. The method is slower because the four coefficients must be computed during each integration step.

# Multi-compartment models

## Geometry

## The `Axial` synapse

## The `slice` method

# Visualization 

## Using the built-in `plot` function

## Manipulating models

### Writing custom plot functions that can be manipulated 


# Creating new conductances

## Understanding the `conductance` class

## Manually  

## Using the `conductance` class

## Where should I put them?

# Creating new synapses

## Understanding the `synapse` class

# Creating new mechanisms 

## Understanding the `mechanism` class 


# Diving deeper: `cpplab`


# Troubleshooting 

## Controlling Verbosity 

```
x.verbosity = 99;
```

Need a table to tell the user what each value of verbosity does. 


## Resetting the `cpplab` path cache

You can rebuild the cache with:

```matlab
cpplab.rebuildCache()
```

If that doesn't work, you can forcibly delete the cache with:

```matlab
delete(which('paths.cpplab'))
```

# Limitations and gotchas

## Mixing object types 

## Compartment types 

<!--stackedit_data:
eyJoaXN0b3J5IjpbMTgzMTkzMTY2NSwtNDc3OTc0MzQ2LDE3NT
Y3MjA4NDAsLTE3NDkxODEwMSwxNTQ3NjkxODQsMjI0MDYzOTcs
LTg3MjU1Njg5NSwyMDY3MDcwODcyLDE4MTQ1MzI0MSw0MzI5MD
E0NTAsNzIyMTgzMTczLDY3MTkwODUyMSwtMTU5OTgzOTIxNiwt
Mjg3ODUxMjYxLC03NDQyMzM5MzIsLTEwMzgzODUxMTEsLTQzOT
gyMDg2OF19
-->