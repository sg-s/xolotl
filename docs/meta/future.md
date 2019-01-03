This document describes what the future roadmap for xolotl looks 
like. 

## Loose ends

Some features in xolotl currently are not fully polished, and
could do with some refinement, or are incompletely implemented.

### Multi-compartment support

Right now, support for multi-compartment models is very limited. 
In the future, we aim to:

* have robust solvers for branched geometries (currently only unbranched cables are supported)
* allow for voltage-clamping individual compartments in a multi-compartment model 


## New features 

The following features are planned for inclusion in xolotl, though
no work has been done on them yet. 

### Stand-alone integration 

Since xolotl is written entirely in C++, it should be possible
to create stand-alone binaries that can be run without MATLAB. 
This requires some minor tweaks, and including a dummy "mex.h",
but these are easily solved problems. 

Once this architecture is in place, stand alone binaries can be run
on any architecture, without needed MATLAB - an advantage for 
large parameter scans or running on computers that you don't
have root access to, or where MATLAB licenses aren't available. 

### Multiple compartment types 

Right now, all components derive from abstract classes except the
compartment object, of which there is only one type. It should be 
possible to have multiple different types of compartments, so we
can mix and match compartment types. 

### Universal support for Runge-Kutta solvers

Runge-Kutta solver support is limited right now. It should be
possible to use RK4 solvers on any model. 

### Adaptive time step solvers 

Using an adaptive time step solver should speed up simulation
tremendously. 

### Unit support 

MATLAB has built-in unit support, which is admittedly a little
clunky. We should either use this, or build our own unit type
system and support units out of the box in xolotl. 

## Maintainer commitment 

Currently, xolotl is being maintained by Srinivas 
Gorur-Shandilya and Alec Hoyland, who will be maintaining it
till August 2022. If you're interested in becoming a 
maintainer, [get in touch](https://srinivas.gs#contact). 