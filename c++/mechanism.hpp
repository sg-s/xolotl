// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Abstract class for defining mechanisms 
// mechanisms are tied to specific conductances,
// or synapses. You can't have "naked" mechanisms
// that have nothing to control. 
// 
// mechanisms are stored in compartments, and are
// asked to integrate by the compartment they are in
// which provides them the calcium error signal
// and the timestep
//
// this abstract class the following elements, which
// all mechanisms will always have:
// channel        (a pointer to the conductance it controls)
// mechanism_idx (an integer identifying the mechanism 
//                 within the compartment it is in)
// 
// everything else assumes something about the 
// mechanism of the mechanism, so should be in its own
// sub class


#ifndef MECHANISM
#define MECHANISM
#include <cmath>
#include <string>
using std::string;
class conductance;
class synapse;


class mechanism {
protected:
    conductance* channel; // pointer to conductance that this regulates
    synapse* syn; // pointer to synapse that this regulates 
    compartment * comp; // pointer to compartment that it is in

public:

    // store the type of the thing being controlled
    // as a string 
    string controlling_class;

    int mechanism_idx;

    // also store the parameters of the 
    // compartment that it is physically located in
    double container_A;
    double container_vol;

    mechanism()
    {
        channel = NULL; // null pointer 
        syn = NULL; 
        comp = NULL;
    }
    
    ~mechanism() {}
    
    virtual void integrate(double) = 0;
    virtual int getFullStateSize(void) = 0;
    virtual int getFullState(double*, int) = 0;
    virtual double getState(int) = 0;

};





#endif
