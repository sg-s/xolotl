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
    string controlling_class;

    int mechanism_idx;

    // also store the parameters of the
    // compartment that it is physically located in
    double container_A;
    double container_vol;

    double dt;
    double temperature;
    double temperature_ref;

    double verbosity;

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
    virtual int getFullStateSize(void) = 0;
    virtual int getFullState(double*, int) = 0;
    virtual double getState(int) = 0;
    virtual string getClass(void) = 0;

    virtual void checkSolvers(int) = 0;

    // connection methods
    virtual void connect(compartment *);
    virtual void connect(conductance *);
    virtual void connect(synapse *);


};

/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrate"
method, which will be used instead of this.
*/
void mechanism::integrate() {
    mexErrMsgTxt("[mechanism] Unimplemented integration method\n");
}


/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.
*/
void mechanism::connect(compartment* comp_) {
    mexErrMsgTxt("[mechanism] This mechanism cannot connect to a compartment object");
}

/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.
*/
void mechanism::connect(conductance* cond_) {
    mexErrMsgTxt("[mechanism] This mechanism cannot connect to a conductance object");
}

/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "connect"
method, which will be used instead of this.
*/
void mechanism::connect(synapse* syn_) {
    mexErrMsgTxt("[mechanism] This mechanism cannot connect to a synapse object");
}



/*
This virtual method is a placeholder method of mechanism that does
nothing except throw an error. If your mechanim is properly 
written, this will not be run (and therefore the error will
not be thrown) because your mechanism will define a "integrateMS"
method, which will be used instead of this.
*/
void mechanism::integrateMS(int k, double V, double Ca) {
    mexErrMsgTxt("[mechanism] Unimplemented multi-step integration method\n");
}



#endif
