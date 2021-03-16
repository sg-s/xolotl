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
#include <vector>

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


    // this int identifies the location of the start in mech_state
    // in the parent compartment from where we can start reading out values corresponding to this mechanism
    int mech_state_offset = 0;


    mechanism()
    {
        channel = nullptr;
        syn = nullptr;
        comp = nullptr;
    }



    ~mechanism() {}

    virtual void integrate(void);
    virtual void integrateMS(int, double, double);

    

    virtual void checkSolvers(int);

    // connection methods
    virtual void connectCompartment(compartment*);
    virtual void connectConductance(conductance*);
    virtual void connectSynapse(synapse *);



    virtual void init(void);



    // methods for other classes to pass data
    virtual int getFullState(double*, int);
    virtual double getState(int);
    double getPrevState(int);


    // helper methods to allow mechanisms to self-assemble
    // based on names and types
    std::vector<mechanism*> findMechanismsOfTypeControlling(std::string, std::string); 
    std::vector<mechanism*> findMechanismsOfType(std::string get_this_type);
    std::vector<mechanism*> findMechanismsControlling(std::string);
    mechanism* findMechanismNamedControlling(std::string, std::string);


};



/*
This virtual method is a placeholder method of mechanism that does
nothing. If your mechanism is properly 
written, this will not be run because your mechanism will define a "integrate"
method, which will be used instead of this.

It's perfectly OK to have a mechanism that does nothing. For example, such a mechanism would essentially be a struct storing some value. There is no performance penalty for such mechanisms.

*/
void mechanism::integrate() {
    // do nothing
}



/*
This virtual method is a placeholder method of mechanism that does
nothing. If your mechanism is properly 
written, this will not be run because your mechanism will define a "integrateMS"
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


/* The getState method is a very important method of mechanism
and is used for two primary purposes:

1. To pass information from one mechanism to another
2. To read out mechanism states and output time-series 

Every mechanism can define as many "states" as it wants. Make sure 
that the number of states (defined in the "fullStateSize" property)
matches the actual number of states. xolotl at some point will 
call your mechanism with various integers up to fullStateSize-1
so make sure you return something sensible. 

The typical way to write a getState method is to use a switch 
statement, because the argument is an integer, and because 
switches are fast. 

*/
double mechanism::getState(int i) {
    return 0;
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



