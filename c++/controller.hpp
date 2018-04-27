// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Abstract class for defining controllers 
// controllers are tied to specific conductances,
// or synapses. You can't have "naked" controllers
// that have nothing to control. 
// 
// controllers are stored in compartments, and are
// asked to integrate by the compartment they are in
// which provides them the calcium error signal
// and the timestep
//
// this abstract class the following elements, which
// all controllers will always have:
// channel        (a pointer to the conductance it controls)
// controller_idx (an integer identifying the controller 
//                 within the compartment it is in)
// 
// everything else assumes something about the 
// mechanism of the controller, so should be in its own
// sub class


#ifndef CONTROLLER
#define CONTROLLER
#include <cmath>
#include <string>
using std::string;
class conductance;
class synapse;


class controller {
protected:
    conductance* channel; // pointer to conductance that this regulates
    synapse* syn; // pointer to synapse that this regulates 


public:

    // store the type of the thing being controlled
    // as a string 
    string controlling_class;

    int controller_idx;

    // also store the parameters of the 
    // compartment that it is physically located in
    double container_A;
    double container_vol;

    controller()
    {
        channel = NULL; // null pointer 
        syn = NULL; 
    }
    
    ~controller() {}
    
    virtual void integrate(double, double) = 0;
    virtual int getFullStateSize(void) = 0;
    virtual int getFullState(double*, int) = 0;
    virtual double getState(int) = 0;

};





#endif
