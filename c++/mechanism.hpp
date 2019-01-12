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

    virtual void checkSolvers(int) = 0;

    // connection methods
    virtual void connect(compartment *) = 0;
    virtual void connect(conductance *) = 0;
    virtual void connect(synapse *) = 0;


};

/*
Since mechanisms can be just about anything, the abstract
mechanism class only implements two methods. 
This method is used to integrate the mechanism under default 
conditions (single-step integration). 
*/
void mechanism::integrate() {
    mexErrMsgTxt("[mechanism] Unimplemented integration method\n");
}


/*
Since mechanisms can be just about anything, the abstract
mechanism class only implements two methods. 
This method is used to integrate the mechanism when a multi-step
solver is requested. 
*/
void mechanism::integrateMS(int k, double V, double Ca) {
    mexErrMsgTxt("[mechanism] Unimplemented multi-step integration method\n");
}   



#endif
