/*

This document describes the "mechanism" C++ class.
This class describes objects that are mechanisms, and
can be used to represent any sort of mechanism or dynamical
system that affects compartments, conductances or synapses,
or even other mechanisms.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| yes |  nothing | compartment |



*/


#ifndef MECHANISM
#define MECHANISM
#include <cmath>
#include <string>
using std::string;
class compartment;
class conductance;
class synapse;


class mechanism {
protected:
    conductance* channel; // pointer to conductance that this regulates
    synapse* syn; // pointer to synapse that this regulates


public:

    compartment * comp; // pointer to compartment that it is in

    // store the type of the thing being controlled
    // as a string
    string controlling_class = "unset";
    string name = "unset";
    string mechanism_type = "unset";

    int mechanism_idx;

    // also store the parameters of the
    // compartment that it is physically located in
    double container_A;
    double container_vol;

    double dt;
    double temperature;
    double temperature_ref;

    int verbosity = -1;

    int fullStateSize = 0;

    mechanism()
    {
        // null pointers to all
        // connectors for safety
        channel = NULL;
        syn = NULL;
        comp = NULL;
    }

    ~mechanism() {}

    virtual void integrate(void);
    virtual void integrateMS(int, double, double);

    virtual int getFullState(double*, int);
    virtual double getState(int);

    virtual void checkSolvers(int);

    // connection methods
    virtual void connectCompartment(compartment*);
    virtual void connectConductance(conductance*);
    virtual void connectSynapse(synapse *);

    virtual void init(void);


};

/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrate"
method, which will be used instead of this.
*/
void mechanism::integrate() {
    // do nothing
}


/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.
*/
void mechanism::connectCompartment(compartment* comp_) {
    string txt = "Error using ";
    txt += name;
    txt += ". This mechanism cannot connect to a compartment object";
    mexErrMsgTxt(txt.c_str());
}

/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.
*/
void mechanism::connectConductance(conductance* cond_) {
    string txt = "Error using ";
    txt += name;
    txt += ". This mechanism cannot connect to a conductance object";
    mexErrMsgTxt(txt.c_str());
}

/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.
*/
void mechanism::connectSynapse(synapse* syn_) {
    string txt = "Error using ";
    txt += name;
    txt += ". This mechanism cannot connect to a synapse object";
    mexErrMsgTxt(txt.c_str());
}



/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanism is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrateMS"
method, which will be used instead of this.
*/
void mechanism::integrateMS(int k, double V, double Ca) {
    // do nothing
}



/*
This virtual method is a placeholder initialization method. Initialization 
methods are called after the model has been constructed, and all components
have been connected, but before the model starts the simulation. This is a
good place to put code that you need to run once before the simulation. 
*/

void mechanism::init() {
    // do nothing
}


/* This virtual method is a placeholder. If you want to read values
out of your mechanism, make sure this returns something sensible. 
*/
double mechanism::getState(int i) {
    return 0;
}



/* This virtual method does nothing, but can return as many dynamic
variables as you want 
*/

int mechanism::getFullState(double* cont_, int i) {
    return i;
}


/* This virtual method only allows runs if solver_order is 0
If your mechanism supports other solver orders, say so in your method
to override this. 
*/

void mechanism::checkSolvers(int k) {

    if (k == 0) {return;}
    else {
        string txt = "Error using ";
        txt += name;
        txt += ". This mechanism does not support this solver order. Try using x.solver_order = 0;";
        mexErrMsgTxt(txt.c_str());
    }
}

#endif
