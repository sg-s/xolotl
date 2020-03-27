
This document will describe how to create your own
components in xolotl. Currently, you can create your
own conductances, mechanisms and synapses.


## Creating new conductances

### Create a new conductance using the [`conductance`](../../reference/conductance) class

!!! Note "Demo"
    A worked example showing how the `conductance` class can be used to generate new channels exists in `../xolotl/examples/demo_conductance`.


First, create a `conductance` object.

```matlab
newCond = conductance;
```

Remember that you  can always see all the
properties of the object  using `properties(newCond)`.

Then, the activation steady-state function `m_inf`,
the inactivation steady-state function `h_inf`,
the activation time constant function `tau_m`, and
the inactivation time constant function `tau_h`
must be defined, as function handles.

!!! Note "Instantaneous Kinetics"
    If your conductance has instantaneous kinetics, you can set `tau_m` to `@(V, Ca) 0`.
    Xolotl will automatically use a different integration scheme
    that updates `m` to its instantaneous steady-state value.

For example,

```matlab
newCond.m_inf = @(V,Ca) 1.0 / (1.0 + exp((V-20.0)/5.0));
% and so on
```

You must also set whether this is a Calcium conductance.

```matlab
newCond.is_Ca = false;
```

Specify the exponents of the activation
`p` and (de)inactivation `q` gating variables
in the current equation.

```matlab
newCond.p = 4;
newCond.q = 1;
```

Finally, you must set the default reversal potential.

```matlab
newCond.default_E = -80;
```

You can also set the default activation and
inactivation gating variable values with
`newCond.default_m` and `newCond.default_h`.

We're done! Now we can automatically generate a `C++` file:


```matlab
newCond.generateCPPFile('condName')
```

where `'condName'` is what you want to name the conductance.
A `C++` header file will be generated at `../xolotl/c++/conductances/custom/condName.hpp`.
You can now use this conductance like any other, e.g. `x.comp.add('custom/condName', 'gbar', 10)`.


### Creating a new conductance by hand

It is reasonably straightforward to create your own conductance
by hand (i.e., to create the C++ file yourself). A good starting
point is typically to look at the C++ files for existing
conductances to get a sense of what the class looks like.

This is what a skeleton of a new conductance file would look like:

```C++

#ifndef NEWCOND
#define NEWCOND
#include "conductance.hpp"

//inherit conductance class spec
class NewCond: public conductance {

public:

    // specify parameters + initial conditions
    NewCond(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }


        if (isnan (E)) { E = 30; }

        // specify exponents of m and h
        p = 3;
        q = 1;

        // allow this channel to be approximated?
        approx_m = 1; // or 0 if not
        approx_h = 1;
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string NewCond::getClass(){return "NewCond";}


double NewCond::m_inf(double V, double Ca) {return ...;}
double NewCond::h_inf(double V, double Ca) {return ...;}
double NewCond::tau_m(double V, double Ca) {return ...;}
double NewCond::tau_h(double V, double Ca) {return ...;}


#endif


```

!!! warning "conductance" vs. "conductance"
    Note that there are two different classes called "conductance". One of them is an abstract C++ class that all object of type conductance must inherit from. The other is a MATLAB class that is used to generate conductances automatically. When we're talking about C++ code, we're referring to the abstract C++ class.


#### Why aren't there any integration routines?

Because models for conductances are so stereotyped, integration
routines for them are specified in the conductance class. If we look at the [code for the conductance class](https://github.com/sg-s/xolotl/blob/master/c%2B%2B/conductance.hpp), we see this:

```C++
    virtual void integrate(double, double);
    virtual void integrateMS(int, double, double);
```

which means that while the conductance class can integrate
models on its own, you can also override it with your own
integration routines if you so which, and that will be used.


## Creating new mechanisms

Because mechanisms are so generic, the only way to make new
mechanisms is to write new C++ files. As with conductances,
new mechanisms inherit from the abstract C++ class called "mechanism" and it is typically straightforward to write a new mechanism.

Here's what a skeleton for a new mechanism would look like:

```C++

#ifndef NEWMECH
#define NEWMECH
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class NewMech: public mechanism {

protected:
public:

    // parameters
    double A = 0;
    double B = 1;

    // constructor
    NewMech(double A_, B_)
    {
        A = A_;
        B = B_;
        controlling_class = "unset";
    }

    // declare methods
    void checkSolvers(int);

    void integrate(void);
    void integrateMS(int, double, double);

    double NewMechCustomMethod(double);

    // these connector methods are needed so that
    // we know how the mechanism interacts with other
    // components
    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    // these methods allow reading out of the mechanism
    // state
    int getFullStateSize(void);
    int getFullState(double * mech_state, int idx);
    double getState(int);

};


double NewMech::getState(int idx){return std::numeric_limits<double>::quiet_NaN();}

// specify the dimensions of your mechanism
// this size must match the getState method
int NewMech::getFullStateSize(){return 0; }

// this does nothing since our state size is 0
// otherwise we should increment idx, and
// fill in values in the mech_state array
int NewMech::getFullState(double *mech_state, int idx) {
    return idx;
}

// connection methods
// we allow the mechanism to connect to a compartment
void NewMech::connect(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}

// disallow other connections
void NewMech::connect(conductance* cond_) {
    mexErrMsgTxt("[NewMech] This mechanism cannot connect to a conductance object");
}

void NewMech::connect(synapse* syn_) {
    mexErrMsgTxt("[NewMech] This mechanism cannot connect to a synapse object");
}


// specify how we integrate it with the default
// solver order (a single-step integration)
void NewMech::integrate(void) {
    // insert integration routine here
}

// this method can hold anything, and you can have
// as many custom methods as you want
double NewMech::NewMechCustomMethod(double X_) {
    // insert your code here
}

// Runge-Kutta 4 integrator
// you don't have to code this...see checkSolvers
void NewMech::integrateMS(int k, double V, double Ca_) {
    if (k == 4){return;}
   // insert RK4 code here
}

// throw an error to disallow solver_orders
// 0 must always work, though
void NewMech::checkSolvers(int k) {
    if (k == 0){
        return;
    } else if (k == 4){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech] unsupported solver order\n");
    }
}

#endif

```

## Creating new synapses

Creating new synapses is similar to the process of
creating new conductances. New synapses inherit
from the abstract `C++` class `synapse`.

Here is a skeleton for a new synapse class that you
can fill out for yourself.

```C++
// here is an example of a synapse named NewSynapse
// we define the header file and include the synapse class
#ifndef NEWSYNAPSE
#define NEWSYNAPSE
#include "synapse.hpp"

// define the class for the specific synapse here
class NewSynapse: public synapse {

public:


    // here is the constructor function
    // it accepts the maximal conductance and a single state variable
    NewSynapse(double gbar_, double s_)
    {
        gmax = g_;
        s = s_;

        // we could make it accept the reversal potential too
        // but here we have set it within the function definition
        E = -80.0;

        // set up defaults in case g_ and s_ aren't specified in MATLAB during construction
        if (isnan (s)) { s = 0; }
        if (isnan (gmax)) { gmax = 0; }
        is_electrical = false;
    }

    // all of these functions are needed for all synapses
    void integrate(void);
    void integrateMS(int, double, double);
    void checkSolvers(int);

    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
    int getFullState(double*, int);

    // these functions are specific to this synapse
    double s_inf(double);
    double tau_s(double);
    double sdot(double, double);

};

// this function returns the state size of the synapse
// which should be the number of state variables
// from this synapse plus one (for the synaptic current)
int NewSynapse::getFullStateSize() {
    return 2;
}

// define the steady-state gating function for this synapse
double NewSynapse::s_inf(double V_pre) {
    return 1.0/(1.0+exp((Vth - V_pre)/Delta));
}


double NewSynapse::tau_s(double sinf_) {
    // this could hold some arbitrary function
}


double NewSynapse::sdot(double V_pre, double s_) {
    // define your own function here
}


void NewSynapse::integrate(void) {
    // define your integration routing here

}


// determine which solver to use
// 0 should always be supported
void NewSynapse::checkSolvers(int k){
    if (k == 0) {
        return;
    }
    mexErrMsgTxt("[NewSynapse] Unsupported solver order\n");
}

// return the state variable, and the current
int NewSynapse::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // the synaptic current in the post-synaptic compartment
    // is also computed here
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}

// the connect function defines how to wire
// two compartments together
void NewSynapse::connect(compartment *pcomp1_, compartment *pcomp2_) {
    pre_syn = pcomp1_;
    post_syn = pcomp2_;

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}

#endif
```

## Where should I put them?
<a name="whereshouldIputthem"></a>

All conductances (and any other network component) are
defined by an `.hpp` header file. **You can save your
new conductance anywhere within your MATLAB path.**


## How are built-in C++ files organized?

We organize our conductance files in the xolotl
directory under `xolotl/c++/conductances/`. Within that
directory, conductances are grouped by the surname of the
first author on the paper from where they were sourced.
For example, conductances from Liu *et al.* 1998 can
be found in `xolotl/c++/conductances/liu/`.

If an author, such as Farzan Nadim or Cristina Soto-Trevino,
happens to have written more than one paper from which we have
extracted models, the last two digits of the paper publishing year
are appended to the author name (*e.g.* `../nadim98`).
If there are multiple papers in a single year, lowercase alphabetical indices are used (*e.g.* `../golowasch01a`).

If a paper, such as Soplata *et al.* 2017 describes
multiple channels of a single type in different cell
types (*e.g.* thalamocortical relay and thalamic reticular
 cells), then full-word descriptions can be used, such as
(`../soplata/thalamocortical`).
