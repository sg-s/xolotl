/*

This document describes the "compartment" C++ class.
This class describes objects that are compartments, and 
can be used to represent neurons and parts of neurons. 

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| no |  conductance, mechanism, synapse | network |



*/

#ifndef COMPARTMENT
#define COMPARTMENT
#include <cmath>
#include <vector>
#include "conductance.hpp"
#include "synapse.hpp"
#include "mechanism.hpp"
class network;

#define F 96485
#define pi 3.1416

using namespace std;

class compartment
{
protected:

    vector<conductance*> cond; // pointers to all conductances in compartment
    vector<synapse*> syn; // pointers to synapses onto this neuron.
    vector<mechanism*> cont; // pointers to mechanisms
    vector<int> mechanism_sizes; // stores sizes of each mechanism's full state
    vector<int> synapse_sizes; // stores sizes of each mechanism's full state

    // vector that will store the axial synapses
    vector <synapse*> axial_syn;

    // voltage and other state variables (calcium, ..
    double sigma_g;
    double sigma_gE;
    double V_inf;

public:


    // core dynamic variables
    double V = -60;
    double Ca;
    double k_V[4] = {0,0,0,0};
    double k_Ca[4] = {0,0,0,0};


    double approx_channels = 0;
    double dt;
    double temperature;
    double temperature_ref = 11;

    // some housekeeping parameters
    // that will be useful in the
    // Crank-Nicholson integration scheme
    double c_;
    double f_;
    double delta_V;
    double i_Ca_prev = 0;
    double Ca_prev;
    double V_prev;

    double V_clamp = 0; // stores the voltage that it is clamped to

    double verbosity = 0;

    double RT_by_nF;

    // this int stores an integer that indicates
    // the hierarchy of this compartment in a multi-comp
    // neuron tree. tree_idx of 0 means it is a soma
    // compartment
    double tree_idx;

    // this number stores a numeric value
    // that corresponds to the neuron # of this compartment
    // in the whole network. automatically assigned
    double neuron_idx;

    // pointers to upstream and downstream compartments
    // (will be generated on initialization)
    compartment * upstream;
    compartment * downstream;

    // conductances downstream and upstream
    // will be generated on initialization
    double downstream_g;
    double upstream_g;


    // neuron parameters

    // size
    // geometry is assumed to be a cylinder.
    // if not, you must specify the area and
    // volume
    double A = .0628;
    double vol;
    double radius; // mm
    double len; // mm
    double shell_thickness; // mm


    double Cm = 10; // specific capacitance


    double Ca_target; // for homeostatic control
    double Ca_out = 3000;


    // stores the average Ca over the integration
    // window. useful for quickly determining
    // if integral control has worked without
    // pulling out the full trace
    double Ca_average;


    double E_Ca;
    double i_Ca; // specific calcium current (current/area. nA/mm^2)
    double I_ext; // all external currents are summed here
    double I_clamp; // this is the current required to clamp it
    int n_cond; // this keep tracks of the # channels
    int n_cont; // # of mechanisms
    int n_syn; // # of synapses
    int n_axial_syn;

    int solver_order = 0;

    // constructor with all parameters
    compartment(double V_, double Ca_, double Cm_, double A_, double vol_,  double Ca_target_, double Ca_average_, double tree_idx_, double neuron_idx_, double radius_, double len_, double shell_thickness_, double Ca_out_)
    {

        // core dynamic variables
        V = V_;
        Ca = Ca_;


        Ca_out = Ca_out_;

        sigma_g = 0;
        sigma_gE = 0;
        V_inf = 0;

        // geometry
        vol = vol_;
        A = A_;
        radius = radius_;
        shell_thickness = shell_thickness_;
        len = len_;

        // membrane props
        Cm = Cm_;


        // calcium
        Ca_target = Ca_target_;
        Ca_average = Ca_average_;
        Ca_average = 0; // reset it every time


        // housekeeping
        tree_idx = tree_idx_;
        neuron_idx = neuron_idx_;


        // defaults
        if (isnan (Ca_out)) { Ca_out = 3000;}
        if (isnan (A)) { A = .0628;}
        if (isnan (V)) { V = -60; } // mV
        if (isnan (Ca)) { Ca = .05; }

        // if (isnan (Ca_target)) { Ca_target = Ca_in; }

        if (!isnan(len) && !isnan(radius)) {
            // radius and length
            // are provided, and use
            // cylindrical geometry
            // override defaults if need be
            A = 2*pi*radius*len; // + 2*pi*radius*radius;
            vol = pi*radius*radius*len;

            if (!isnan(shell_thickness))
            {
                double inner_radius = radius - shell_thickness;
                if (inner_radius > 0)
                {
                    vol = pi*len*(radius*radius - inner_radius*inner_radius);
                }
            }
        }


        // housekeeping
        E_Ca = 0; // because this will be computed from the Nernst eq. 
        i_Ca = 0; // this is the current density (nA/mm^2)
        n_cond = 0;
        n_cont = 0;
        n_syn = 0;
        n_axial_syn = 0;
        upstream = NULL;
        downstream = NULL;
        upstream_g = 0;
        downstream_g = 0;

        f_ = 0;
        c_ = 0;
        delta_V = 0;

    }

    // begin function declarations

    // methods to add things to compartment
    void addConductance(conductance*);
    void addSynapse(synapse*);
    void addAxial(synapse*);
    void addMechanism(mechanism*);

    // integration methods
    void integrateMS(int);
    void integrateMechanisms(void);
    void integrateChannels(void);
    void integrateSynapses(void);
    void integrateVoltage(void);
    void integrateV_clamp(double);

    // methods for integrating using Crank-Nicholson
    // and methods for multi-compartment models
    double getBCDF(int);
    void integrateCNFirstPass(void);
    void integrateCNSecondPass(void);
    void resolveAxialConductances(void);

    // methods to retrieve information from compartment
    int getFullMechanismState(double*, int);
    int getFullCurrentState(double*, int);
    int getFullSynapseState(double*, int);
    int getFullMechanismSize(void);
    int getFullSynapseSize(void);

    mechanism* getMechanismPointer(int);
    mechanism* getMechanismPointer(const char*); // overloaded
    compartment* getConnectedCompartment(int);
    conductance* getConductancePointer(const char*);
    conductance* getConductancePointer(int);

    synapse* getSynapsePointer(int);
    

    void checkSolvers(int);


};


/*
This method adds an axial synapse to this compartment. 
It updates the `n_axial_syn` property of this class
so that every compartment can keep track of how many
axial synapses are connected to it. 

**See Also**

* [addSynapse](./compartment.md#addsynapse)
* [addSynapse](./compartment.md#addmechanism)
* [addConductance](./compartment.md#addconductance)


*/
void compartment::addAxial(synapse *syn_) {
    axial_syn.push_back(syn_);
    n_axial_syn++;
}

/*
This method adds a conductance object to this compartment. 
It does the following things:

1. Adds a pointer to the conductance to a vector of pointers called `cond`
2. Updates various attributes of the conductance like verbosity, etc.
3. Runs some initializaiton code (e.g. building look-up-tables in the conductance)
4. Calls the `connect` method of that conductance with a pointer to this compartment. 

**See Also**

* [addSynapse](./compartment.md#addaxial)
* [addSynapse](./compartment.md#addmechanism)
* [addConductance](./compartment.md#addconductance)

*/
void compartment::addConductance(conductance *cond_) {
    cond.push_back(cond_);
    cond_->verbosity = verbosity;
    cond_->buildLUT(approx_channels);
    cond_->dt = dt;
    cond_->temperature_ref = temperature_ref;
    cond_->temperature = temperature;
    cond_->connect(this);

    n_cond++;

    if (verbosity > 0) {
        mexPrintf("[C++] adding conductance of type: %s\n", cond_->getClass().c_str());
    }
}


/*
This method adds a mechanism object to this compartment. 
It does the following things:

1. Adds a pointer to the conductance to a vector of pointers called `cont` 
2. Updates various attributes of the conductance like verbosity, etc.
3. Tells the mechanism what its ordering in `cont` is by updating `mechanism_idx` in that mechanism object
4. Determines the data frame size of this object by calling `getFullStateSize` and storing this in `mechanism_sizes`

**See Also**

* [addSynapse](./compartment.md#addsynapse)
* [addConductance](./compartment.md#addconductance)

*/

void compartment::addMechanism(mechanism *mech_) {
    // mexPrintf("adding mechanism @  %p\n",mech_);
    mech_->dt = dt;
    // mech_->verbosity = verbosity;
    mech_->temperature = temperature;
    mech_->temperature_ref = temperature_ref;
    cont.push_back(mech_);
    mech_->mechanism_idx = n_cont; // tell the mechanism what rank it has
    n_cont++;

    // also store the mechanism's full state size
    mechanism_sizes.push_back(mech_->getFullStateSize());
}

/*
This method adds a synapse to this compartment. A
pointer to this synapse is stored in the vector `syn` 

**See Also**

* [addSynapse](./compartment.md#addaxial)
* [addSynapse](./compartment.md#addmechanism)
* [addConductance](./compartment.md#addconductance)

*/
void compartment::addSynapse(synapse *syn_) {
    syn_->dt = dt;
    syn_->verbosity = verbosity;
    syn.push_back(syn_);
    n_syn ++;

    // also store the synapse's full state size
    synapse_sizes.push_back(syn_->getFullStateSize());
}

/*

This method is called when a non-default solver order
is requested. The compartment checks that it can support
this solver order, and then asks every component contained
in it if they can support this solver order. This ensures
that the solver order can actually be used, because if
any component cannot support this solver order (because
those integration routines have not been written), then
they can throw an error, aborting the simulation. 

*/
void compartment::checkSolvers(int solver_order) {

    if (solver_order == 0){
        return;
    } else if (solver_order == 4){
        for (int i=0; i<n_cond; i++) {
            cond[i]->checkSolvers(solver_order);
        }

        for (int i=0; i<n_cont; i++) {
            cont[i]->checkSolvers(solver_order);
        }

        for (int i=0; i<n_syn; i++) {
            syn[i]->checkSolvers(solver_order);
        }

    } else {
        mexErrMsgTxt("[compartment] Unsupported solver order \n");
    }
}

/*
This method is a helper function used in the Crank-Nicholson 
scheme and returns B, C, D and F values as defined in eq.
6.45 of "Theoretical Neuroscience" by Dayan and Abbott
*/
double compartment::getBCDF(int idx){
    if (idx == 0) {
        return 0;
    } else if (idx == 1) {
        // return B
        if (upstream){
            return (upstream_g/Cm);

        } else {
            // no upstream, 1st compartment,
            // return 0
            return 0;
        }

    } else if (idx == 2) {
        // return C
        double C = sigma_g;
        if (downstream) {C += downstream_g;}
        if (upstream) {C += upstream_g;}
        C = -C/Cm;
        return C;

    } else if (idx == 3) {
        // return D
        if (downstream) {
            return (downstream_g/Cm);
        } else {
            // no downstream
            return 0;
        }

    } else if (idx == 4) {
        // return F
       return (sigma_gE + I_ext/A)/Cm;

    } else  {
        return 0;
    }

}


/*
This method returns the pointer to a conductance stored 
in this compartment, identified by its class name.

!!! warning 
    Note that there is another method with the same name that can be called using a integer argument.

*/
conductance* compartment::getConductancePointer(const char* cond_class) {
    conductance* req_cond = NULL;

    for (int i = 0; i < n_cond; i ++)
    {
        if ((cond[i]->getClass()) == cond_class)
        {
            req_cond = cond[i];
        }
    }

    return req_cond;
}

/*
This method returns the pointer to a conductance contained
in this compartment, identified by its numeric index. 

!!! warning 
    Note that there is another method with the same name that can be called using a char argument.

*/
conductance * compartment::getConductancePointer(int cond_idx){
    if (cond_idx < n_cond) { return cond[cond_idx];} 
    else { return NULL; } 
}


/*
This method returns a pointer to a compartment that is linked
via axial synapses to this compartment. 
*/
compartment* compartment::getConnectedCompartment(int idx) {
    compartment* neighbour = NULL;
    if (idx > n_axial_syn) {return neighbour;}
    neighbour = axial_syn[idx]->pre_syn;
    return neighbour;
}

/*
This method is used to read out the full dynamical state of 
all channels, and to store it in a large array for output. 

Since  C++ doesn't support returning multiple values, this function 
works like this:

* it is first called with a pointer to the array, and an integer
specifying where it should write values to
* it writes as many values as it wants to, to the array, and returns a new integer indicating the next empty location in the array

**See Also**

* [getFullSynapseState](./compartment.md#getfullsynapsestate)

*/
int compartment::getFullCurrentState(double *cond_state, int idx)
{
    for (int i = 0; i < n_cond; i ++)
    {
        cond_state[idx] = cond[i]->getCurrent(V);
        idx ++;
    }
    return idx;
}

/*
This method returns the full dimension size of all the mechanisms
in this compartment. The full size is calculated on the fly
by recursively asking all the mechanisms in this compartment
what their data dimension is, and adding up all those numbers. 

**See Also**

* [getFullSynapseSize](./compartment.md#getFullSynapseSize)

*/
int compartment::getFullMechanismSize(void) {
    int full_size = 0;
    for (int i=0; i<n_cont; i++)
    {
        full_size += cont[i]->getFullStateSize();
    }
    return full_size;
}


/*
This method is used to read out the full dynamical state of 
all mechanisms, and to store it in a large array for output. 

Since  C++ doesn't support returning multiple values, this function 
works like this:

* it is first called with a pointer to the array, and an integer
specifying where it should write values to
* it writes as many values as it wants to, to the array, and returns a new integer indicating the next empty location in the array

**See Also**

* [getFullSynapseState](./compartment.md#getfullsynapsestate)
* [getFullCurrentState](./compartment.md#getfullcurrentstate)

*/
int compartment::getFullMechanismState(double *mech_state, int idx)
{
    for (int i = 0; i < n_cont; i ++)
    {

        cont[i]->getFullState(mech_state, idx);
        idx += mechanism_sizes[i];

    }
    return idx;
}


/*
This method returns the full dimension size of all the synapses
in this compartment. The full size is calculated on the fly
by recursively asking all the synapses in this compartment
what their data dimension is, and adding up all those numbers. 

**See Also**

* [getFullMechanismSize](./compartment.md#getFullMechanismSize)

*/
int compartment::getFullSynapseSize(void) {
    int full_size = 0;
    for (int i=0; i<n_syn; i++)
    {
        full_size += syn[i]->getFullStateSize();
    }
    return full_size;
}


/*
This method is used to read out the full dynamical state of all 
synapses, and to store it in a large array for output. 

Since C++ doesn't support returning multiple values, this function 
works like this:

* it is first called with a pointer to the array, and an integer
specifying where it should write values to
* it writes as many values as it wants to, to the array, and returns a new integer indicating the next empty location in the array

**See Also**

* [getFullCurrentState](./compartment.md#getfullcurrentstate)

*/
int compartment::getFullSynapseState(double *syn_state, int idx) {
    for (int i = 0; i < n_syn; i ++) {
        idx = syn[i]->getFullState(syn_state,idx);
    }
    return idx;
}


/*
This method returns a pointer to a mechanism stored in this
compartment, identified by its numerical index.
*/
mechanism * compartment::getMechanismPointer(int mech_idx){
    if (mech_idx < n_cont) { return cont[mech_idx];} 
    else { return NULL; }   
}

/*
This method returns a pointer to a mechanism that is contained
by this compartment, that is linked to a conductance of a certain
type, also in this compartment. 
*/

mechanism* compartment::getMechanismPointer(const char* cond_class){
    mechanism* req_cont = NULL;

    for (int i = 0; i < n_cont; i ++) {
        if ((cont[i]->controlling_class) == cond_class) {
            req_cont = cont[i];
        }
    }

    return req_cont;
}




/*
This method returns a pointer to a synapse contained by
this compartment, where this synapse is identified by the 
numerical index it was added to this compartment.

*/
synapse * compartment::getSynapsePointer(int syn_idx){
    if (syn_idx < n_syn) { return syn[syn_idx];} 
    else { return NULL; } 
}



/*
This method integrates all the channels in the compartment. This
method is only used when this compartment is not part of a 
multi-compartment model. It simply asks every conductance
object in this compartment to integrate by calling their 
`integrate` methods. 

*/
void compartment::integrateChannels(void) {

    sigma_g = 0.0;
    sigma_gE = 0.0;


    // compute E_Ca
    E_Ca = RT_by_nF*log((Ca_out)/(Ca_prev));

    // integrate all channels
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->integrate(V_prev, Ca_prev);
        sigma_g += cond[i]->g;
        sigma_gE += (cond[i]->g)*(cond[i]->E);
    }

    // update the running total Ca
    Ca_average += Ca;
}


/*
This method is part of the Crank-Nicholson method to solve
for the voltages in a multi-compartment neuron. This is the 
"first pass" down a cable. In simulations, this is called before
integrateCNSecondPass. 

**See Also**

* [integrateCNSecondPass](./compartment.md#integratecnsecondpass)
* [The Crank-Nicholson Method](../../explanation/integration.md)

*/
void compartment::integrateCNFirstPass(void) {
    // intermediate variables
    double b; // b is b for this compartment
    double d; // d is d for the prev compartment

    b = getBCDF(1)*.5*dt;


    // compute c_
    c_ = .5*dt*getBCDF(2);
    if (upstream) {

        d = (upstream->getBCDF(3))*dt*.5;

        // full expression for c_ (eq. 6.54)
        c_ += b*d/(1 - upstream->c_);

    }

    // compute f_
    // first compute f
    double f = getBCDF(4) + getBCDF(2)*V;
    if (upstream) {
        f += getBCDF(1)*(upstream->V);
    }
    if (downstream) {
        f += getBCDF(3)*(downstream->V);
    }
    f = f*dt;

    f_ = f;

    if (upstream) {
        // downstream exists. append terms
        // (eq. 6.55 in Dayan & Abbott)
        f_ += (b*(upstream->f_))/(1 - upstream->c_);
    }

}


/*
This method is part of the Crank-Nicholson method to solve
for the voltages in a multi-compartment neuron. This is the 
"second pass" up a cable. In simulations, this is called after
integrateCNFirstPass. 

**See Also**

* [integrateCNFirstPass](./compartment.md#integratecnFirstpass)
* [The Crank-Nicholson Method](../../explanation/integration.md)
*/
void compartment::integrateCNSecondPass(void) {
    delta_V = f_;
    if (downstream) {
        // downstream exists, use full eq (6.53)
        delta_V += getBCDF(3)*.5*dt*(downstream->delta_V);
    }

    // divide by common denominator
    delta_V = delta_V/(1 - c_);


    V += delta_V;
}

/*
This method integrates all mechanisms in this compartment. It 
simply calls the integrate method on every mechanism in the 
compartment. 
*/
void compartment::integrateMechanisms(void) {
    for (int i=0; i<n_cont; i++) {
        cont[i]->integrate();
    }
}

/*
This method integrates the voltage in this compartment,
and all components contained by this compartment, using 
the Runge-Kutta 4 solver. It iteratively calls the 
`integrateMS` method of all components contained within
this compartment. 
*/
void compartment::integrateMS(int k){


    if (k == 4) {
        // terminal calculations, advance step
        V = V_prev + (k_V[0] + 2*k_V[1] + 2*k_V[2] + k_V[3])/6;
        Ca = Ca_prev + (k_Ca[0] + 2*k_Ca[1] + 2*k_Ca[2] + k_Ca[3])/6;
    }


    // reset some things
    sigma_g = 0;
    sigma_gE = 0;
    i_Ca = 0;

    double V_MS;
    double Ca_MS;

    if (k == 0) {
        // first step
        V_MS = V_prev;
        Ca_MS = Ca_prev;
    } else if (k == 1) {
        V_MS = V_prev + k_V[0]/2;
        Ca_MS = Ca_prev + k_Ca[0]/2;
    } else if (k == 2) {
        V_MS = V_prev + k_V[1]/2;
        Ca_MS = Ca_prev + k_Ca[1]/2;
    } else if (k == 3) {
        V_MS = V_prev + k_V[2];
        Ca_MS = Ca_prev + k_Ca[2];
    }

    E_Ca = RT_by_nF*log((Ca_out)/(Ca_MS));


    // channels
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->integrateMS(k, V_MS, Ca_MS);
        sigma_g += cond[i]->g;
        sigma_gE += (cond[i]->g)*(cond[i]->E);
    }

    //synapses 
    for (int i=0; i<n_syn; i++) {
        syn[i]->integrateMS(k, V_MS, Ca_MS);
        sigma_g += (syn[i]->gmax)*(syn[i]->s)/(1000*A); // now uS/mm^2
        sigma_gE += ((syn[i]->gmax)*(syn[i]->s)*(syn[i]->E)/(1000*A));
    }


    // mechanisms
    for (int i=0; i<n_cont; i++) {
        cont[i]->integrateMS(k, V_MS, Ca_MS);
    }


    //voltage 
    // only compute when k < 4
    if (k == 4) {return;}
    

    k_V[k] = dt*(sigma_gE - sigma_g*V_MS)/Cm;
    

}





/*
This method integrates all synapses in this compartment.
All that this method does is "ask" every synapse to integrate
(by calling their integrate method), and keep track of how they
contribute to `sigma_g` and `sigma_gE`. 

*/
void compartment::integrateSynapses(void) {
    // we treat synapses identically to any other conductance
    for (int i=0; i<n_syn; i++) {
        syn[i]->integrate();
        sigma_g += (syn[i]->g)/(1000*A); // now uS/mm^2
        sigma_gE += ((syn[i]->g)*(syn[i]->E))/(1000*A); // now uS/mm^2
    }
}


/*
This integration method is called when a compartment is 
voltage clamped. Here, the voltage is updated to the clamp
voltage, and the current required to do so is computed and
stored in `I_clamp`.
*/
void compartment::integrateV_clamp(double V_clamp) {

    // calculate I_clamp, and set voltage to the clamped
    // voltage
    double E = exp(-dt/(Cm/(sigma_g)));
    V_inf = (V_clamp - V*E)/(1 - E);
    I_clamp =  A*(V_inf*sigma_g - sigma_gE);

    V = V_clamp;

}


/*
This method integrates the voltage in this compartment, 
assuming this compartment is not part of a multi-compartment 
neuron model, and default solver orders are being used. 

This method implements the exponential Euler method to 
update the voltages in this compartment. 
*/
void compartment::integrateVoltage(void) {

    // compute infinity values for V and Ca
    if (sigma_g == 0) {
        V_inf = V_prev;
    }
    else {
        V_inf = (sigma_gE + (I_ext/A))/sigma_g;
    }

    // integrate V 
    V = V_inf + (V_prev - V_inf)*exp(-dt/(Cm/(sigma_g)));

}






/*
This method is used to "resolve" a multi-compartment model, i.e.,
to figure out which end is which in a cable. Compartments in a multi-compartment model are linked together using `Axial` synapses
and the `tree_idx` property of the compartment is used to indicate
its distance from the soma. 

This method sets the `downstream_g` and `upstream_g` properties
of this compartment based on the other compartments in the model. 

*/
void compartment::resolveAxialConductances(void) {

    if (n_axial_syn == 0) {return;}

    for (int i = 0; i < n_axial_syn; i++)
    {
        if (isnan((axial_syn[i]->pre_syn)->tree_idx)) { continue;}

        if ((axial_syn[i]->pre_syn)->tree_idx > tree_idx)
        {
            // pre_syn of this axial synapse is downstream
            downstream_g = axial_syn[i]->gmax;
        } else {
            // pre_syn of this axial synapse is upstream
            upstream_g = axial_syn[i]->gmax;
        }
    }
}


#endif
