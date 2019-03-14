
This document describes the "network" C++ class.
This class describes the network object, that is used
as an entry point for all other objects.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| no |  compartment | nothing (root) |




## Methods



### checkSolvers

**Function Signature**

```C++
void checkSolvers(void) 
```

**Description**


This method verifies that all components can integrate
using the requested solver order. What this method does
is to call the `checkSolvers` in every compartment,
which in turn calls the `checkSolvers` method in every 
component contained in every compartment. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L88)

-------



### resolveTree

**Function Signature**

```C++
bool resolveTree(void) 
```

**Description**


This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model. 
It does so using the `tree_idx` property in every compartment, 
setting it if need be. 

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise. 




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L111)

-------



### 

**Function Signature**

```C++
        mexPrintf("[C++] resolveTree() called\n");

```

**Description**


This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model. 
It does so using the `tree_idx` property in every compartment, 
setting it if need be. 

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise. 


bool network::resolveTree(void) {
    compartment * connected_comp = NULL;
    bool is_multi_comp = false;
    if (verbosity > 0) {


 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L115)

-------



### startThreads

**Function Signature**

```C++
void startThreads(void) 
```

**Description**


This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model. 
It does so using the `tree_idx` property in every compartment, 
setting it if need be. 

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise. 


bool network::resolveTree(void) {
    compartment * connected_comp = NULL;
    bool is_multi_comp = false;
    if (verbosity > 0) {
        mexPrintf("[C++] network::resolveTree() called\n");
    }


    // ttl =  this_tree_level
    for (int ttl = 0; ttl < n_comp; ttl++) {
        // find all compartments with this_tree_level
        for (int i = 0; i < n_comp; i++) {
            if (isnan(comp[i]->tree_idx)) {continue;}
            if ((comp[i]->tree_idx) != ttl) {continue;}

            // OK, this compartment has the tree level we 
            // are currently interested in 

            if (comp[i]->tree_idx == 0) {
                // mexPrintf("found a soma, calling it = %i\n",n_soma);
                comp[i]->neuron_idx = n_soma;
                n_soma++;
                soma_comp.push_back(comp[i]);
                
            }

            // now go over every synapse that impinges on 
            // this compartment and check if they're Axial
            // and if so get pointers to those presyn compartments

            for (int j = 0; j < comp[i]->n_axial_syn; j ++ ) {
                connected_comp = comp[i]->getConnectedCompartment(j);
                if (!connected_comp){continue;}
                if (isnan(connected_comp->tree_idx)) {
                    double child_tree_idx = ttl+1;

                    // set it 
                    (connected_comp->tree_idx) = child_tree_idx;

                    // wire up stream pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;
                }
                else if ((connected_comp->tree_idx) == (ttl+1)) {
                    // connected_comp already has a tree_idx
                    // possibly manually entered, or from a previous
                    // integrate. if compatible, wire up stream
                    // pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;

                }
            }
        }
    }



    // OK, now we have resolved the tree. 
    // now, we need to mark the downstream_g and 
    // upstream_g for every compartment
    for (int i = 0; i < n_comp; i ++) {
        comp[i]->resolveAxialConductances();
    }


    // go over every compartment, and check that stream
    // pointers and gs match up
    
    if (verbosity > 0)
    {
        for (int i = 0; i < n_comp; i++)
        {
            mexPrintf("---------------\n");
            mexPrintf("this comp tree_idx = %f\n",comp[i]->tree_idx);
            if (comp[i]->downstream)
            {
                mexPrintf("downstream pointer exists\n");
                
            } else {
                mexPrintf("NO downstream pointer\n");
            }   
            mexPrintf("downstream_g =  %f\n", comp[i]->downstream_g);
            if (comp[i]->upstream)
            {
                mexPrintf("upstream pointer exists\n");

            } else {
                mexPrintf("No upstream pointer\n");
            }
            mexPrintf("upstream_g =  %f\n", comp[i]->upstream_g);

        }
    }

    return is_multi_comp;

}




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L217)

-------



### 

**Function Signature**

```C++
            mexPrintf(" starting thread...\n");

```

**Description**


This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model. 
It does so using the `tree_idx` property in every compartment, 
setting it if need be. 

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise. 


bool network::resolveTree(void) {
    compartment * connected_comp = NULL;
    bool is_multi_comp = false;
    if (verbosity > 0) {
        mexPrintf("[C++] network::resolveTree() called\n");
    }


    // ttl =  this_tree_level
    for (int ttl = 0; ttl < n_comp; ttl++) {
        // find all compartments with this_tree_level
        for (int i = 0; i < n_comp; i++) {
            if (isnan(comp[i]->tree_idx)) {continue;}
            if ((comp[i]->tree_idx) != ttl) {continue;}

            // OK, this compartment has the tree level we 
            // are currently interested in 

            if (comp[i]->tree_idx == 0) {
                // mexPrintf("found a soma, calling it = %i\n",n_soma);
                comp[i]->neuron_idx = n_soma;
                n_soma++;
                soma_comp.push_back(comp[i]);
                
            }

            // now go over every synapse that impinges on 
            // this compartment and check if they're Axial
            // and if so get pointers to those presyn compartments

            for (int j = 0; j < comp[i]->n_axial_syn; j ++ ) {
                connected_comp = comp[i]->getConnectedCompartment(j);
                if (!connected_comp){continue;}
                if (isnan(connected_comp->tree_idx)) {
                    double child_tree_idx = ttl+1;

                    // set it 
                    (connected_comp->tree_idx) = child_tree_idx;

                    // wire up stream pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;
                }
                else if ((connected_comp->tree_idx) == (ttl+1)) {
                    // connected_comp already has a tree_idx
                    // possibly manually entered, or from a previous
                    // integrate. if compatible, wire up stream
                    // pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;

                }
            }
        }
    }



    // OK, now we have resolved the tree. 
    // now, we need to mark the downstream_g and 
    // upstream_g for every compartment
    for (int i = 0; i < n_comp; i ++) {
        comp[i]->resolveAxialConductances();
    }


    // go over every compartment, and check that stream
    // pointers and gs match up
    
    if (verbosity > 0)
    {
        for (int i = 0; i < n_comp; i++)
        {
            mexPrintf("---------------\n");
            mexPrintf("this comp tree_idx = %f\n",comp[i]->tree_idx);
            if (comp[i]->downstream)
            {
                mexPrintf("downstream pointer exists\n");
                
            } else {
                mexPrintf("NO downstream pointer\n");
            }   
            mexPrintf("downstream_g =  %f\n", comp[i]->downstream_g);
            if (comp[i]->upstream)
            {
                mexPrintf("upstream pointer exists\n");

            } else {
                mexPrintf("No upstream pointer\n");
            }
            mexPrintf("upstream_g =  %f\n", comp[i]->upstream_g);

        }
    }

    return is_multi_comp;

}


void network::startThreads(void) {

    for (int i = 0; i < n_comp; i++) {
        for (int j = 0; j < comp[i]->n_cond; j++) {


 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L221)

-------



### waitForThreads

**Function Signature**

```C++
void waitForThreads(void) 
```

**Description**


This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model. 
It does so using the `tree_idx` property in every compartment, 
setting it if need be. 

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise. 


bool network::resolveTree(void) {
    compartment * connected_comp = NULL;
    bool is_multi_comp = false;
    if (verbosity > 0) {
        mexPrintf("[C++] network::resolveTree() called\n");
    }


    // ttl =  this_tree_level
    for (int ttl = 0; ttl < n_comp; ttl++) {
        // find all compartments with this_tree_level
        for (int i = 0; i < n_comp; i++) {
            if (isnan(comp[i]->tree_idx)) {continue;}
            if ((comp[i]->tree_idx) != ttl) {continue;}

            // OK, this compartment has the tree level we 
            // are currently interested in 

            if (comp[i]->tree_idx == 0) {
                // mexPrintf("found a soma, calling it = %i\n",n_soma);
                comp[i]->neuron_idx = n_soma;
                n_soma++;
                soma_comp.push_back(comp[i]);
                
            }

            // now go over every synapse that impinges on 
            // this compartment and check if they're Axial
            // and if so get pointers to those presyn compartments

            for (int j = 0; j < comp[i]->n_axial_syn; j ++ ) {
                connected_comp = comp[i]->getConnectedCompartment(j);
                if (!connected_comp){continue;}
                if (isnan(connected_comp->tree_idx)) {
                    double child_tree_idx = ttl+1;

                    // set it 
                    (connected_comp->tree_idx) = child_tree_idx;

                    // wire up stream pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;
                }
                else if ((connected_comp->tree_idx) == (ttl+1)) {
                    // connected_comp already has a tree_idx
                    // possibly manually entered, or from a previous
                    // integrate. if compatible, wire up stream
                    // pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;

                }
            }
        }
    }



    // OK, now we have resolved the tree. 
    // now, we need to mark the downstream_g and 
    // upstream_g for every compartment
    for (int i = 0; i < n_comp; i ++) {
        comp[i]->resolveAxialConductances();
    }


    // go over every compartment, and check that stream
    // pointers and gs match up
    
    if (verbosity > 0)
    {
        for (int i = 0; i < n_comp; i++)
        {
            mexPrintf("---------------\n");
            mexPrintf("this comp tree_idx = %f\n",comp[i]->tree_idx);
            if (comp[i]->downstream)
            {
                mexPrintf("downstream pointer exists\n");
                
            } else {
                mexPrintf("NO downstream pointer\n");
            }   
            mexPrintf("downstream_g =  %f\n", comp[i]->downstream_g);
            if (comp[i]->upstream)
            {
                mexPrintf("upstream pointer exists\n");

            } else {
                mexPrintf("No upstream pointer\n");
            }
            mexPrintf("upstream_g =  %f\n", comp[i]->upstream_g);

        }
    }

    return is_multi_comp;

}


void network::startThreads(void) {

    for (int i = 0; i < n_comp; i++) {
        for (int j = 0; j < comp[i]->n_cond; j++) {
            mexPrintf("network:: starting thread...\n");
            all_threads.push_back(std::thread (&conductance::integrateMT,comp[i]->cond[j]));
        }
    }

   

}




 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L231)

-------



### 

**Function Signature**

```C++
        mexPrintf(" waiting for thread %i\n",i);

```

**Description**


This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model. 
It does so using the `tree_idx` property in every compartment, 
setting it if need be. 

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise. 


bool network::resolveTree(void) {
    compartment * connected_comp = NULL;
    bool is_multi_comp = false;
    if (verbosity > 0) {
        mexPrintf("[C++] network::resolveTree() called\n");
    }


    // ttl =  this_tree_level
    for (int ttl = 0; ttl < n_comp; ttl++) {
        // find all compartments with this_tree_level
        for (int i = 0; i < n_comp; i++) {
            if (isnan(comp[i]->tree_idx)) {continue;}
            if ((comp[i]->tree_idx) != ttl) {continue;}

            // OK, this compartment has the tree level we 
            // are currently interested in 

            if (comp[i]->tree_idx == 0) {
                // mexPrintf("found a soma, calling it = %i\n",n_soma);
                comp[i]->neuron_idx = n_soma;
                n_soma++;
                soma_comp.push_back(comp[i]);
                
            }

            // now go over every synapse that impinges on 
            // this compartment and check if they're Axial
            // and if so get pointers to those presyn compartments

            for (int j = 0; j < comp[i]->n_axial_syn; j ++ ) {
                connected_comp = comp[i]->getConnectedCompartment(j);
                if (!connected_comp){continue;}
                if (isnan(connected_comp->tree_idx)) {
                    double child_tree_idx = ttl+1;

                    // set it 
                    (connected_comp->tree_idx) = child_tree_idx;

                    // wire up stream pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;
                }
                else if ((connected_comp->tree_idx) == (ttl+1)) {
                    // connected_comp already has a tree_idx
                    // possibly manually entered, or from a previous
                    // integrate. if compatible, wire up stream
                    // pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;

                }
            }
        }
    }



    // OK, now we have resolved the tree. 
    // now, we need to mark the downstream_g and 
    // upstream_g for every compartment
    for (int i = 0; i < n_comp; i ++) {
        comp[i]->resolveAxialConductances();
    }


    // go over every compartment, and check that stream
    // pointers and gs match up
    
    if (verbosity > 0)
    {
        for (int i = 0; i < n_comp; i++)
        {
            mexPrintf("---------------\n");
            mexPrintf("this comp tree_idx = %f\n",comp[i]->tree_idx);
            if (comp[i]->downstream)
            {
                mexPrintf("downstream pointer exists\n");
                
            } else {
                mexPrintf("NO downstream pointer\n");
            }   
            mexPrintf("downstream_g =  %f\n", comp[i]->downstream_g);
            if (comp[i]->upstream)
            {
                mexPrintf("upstream pointer exists\n");

            } else {
                mexPrintf("No upstream pointer\n");
            }
            mexPrintf("upstream_g =  %f\n", comp[i]->upstream_g);

        }
    }

    return is_multi_comp;

}


void network::startThreads(void) {

    for (int i = 0; i < n_comp; i++) {
        for (int j = 0; j < comp[i]->n_cond; j++) {
            mexPrintf("network:: starting thread...\n");
            all_threads.push_back(std::thread (&conductance::integrateMT,comp[i]->cond[j]));
        }
    }

   

}


void network::waitForThreads(void) {
    for (int i = 0; i < all_threads.size(); i ++) {


 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L233)

-------



### addCompartment

**Function Signature**

```C++
void addCompartment(compartment *comp_) 
```

**Description**


This method adds a compartment to the network. It does the following things:
1. adds a pointer to the compartment being added to a vector called `comp`
2. Broadcasts certain global parameters like `temperature`, `dt`, etc to all compartments. 
3. Updates `n_comp` to that network knows how many compartments there are.



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L244)

-------



### integrateMS

**Function Signature**

```C++
void integrateMS(double * I_ext_now) 
```

**Description**


This method is used to integrate the network using a
multi-step Runge Kutta solver. This method assumes that
no compartment is being voltage clamped. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L267)

-------



### integrate

**Function Signature**

```C++
void integrate(double * I_ext_now) 
```

**Description**


This method is used to integrate the network using the default
single step solver. Typically, this means using the exponential-
Euler method to integrate conductances and integrate the voltages
and Calcium levels in compartments, though mechanisms can implement
their own integration schemes. Multi-compartment models are 
integrated using the Crank-Nicholson scheme.  

This method assumes that no compartment anywhere is being voltage clamped. 



 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L294)

-------



### integrateClamp

**Function Signature**

```C++
void integrateClamp(double *V_clamp) 
```

**Description**


This method is used to integrate the network using the default
single step solver. Typically, this means using the exponential-
Euler method to integrate conductances and integrate the voltages
and Calcium levels in compartments, though mechanisms can implement
their own integration schemes. Multi-compartment models are 
integrated using the Crank-Nicholson scheme.  

This method assumes that some compartment is being voltage clamped,
and also assumes that no current is being injected into any compartment. 
 


 **Code**

[Click here to view this method's code](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/network.hpp#L395)

-------

