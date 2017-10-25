// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Abstract class for defining controllers 
// controllers are tied to specific conductances,
// so when you construct a controller, you specify
// the conductance it controls
// in addition, controllers need to be added to 
// compartments
// so that the compartment knows they exist and 
// cand ask them to integrate. 
//
// this abstract class the following elements, which
// all controllers will always have:
// channel     (a pointer to the conductance it controls)
// controller_idx (an integer identifying the controller 
//                 within the compartment it is in)
// 
// everything else assumes something about the 
// mechanism of the controller, so should be in its own
// sub class


#ifndef CONTROLLER
#define CONTROLLER
#include <cmath>
class conductance;

class controller {
protected:
    conductance* channel; // pointer to conductance that this regulates
    //compartment* upstream_compartment; // pointer to compartment that is closer to the soma
    //compartment* downstream_compartment; // pointer to compartment that is further from the soma along some process

    //double tau_m; 
    //double tau_g; 
    // double G; 
    // double A; // downstream transport rate 
    //double B; // upstream transport rate
    //double C; // degradation rate

public:

    int controller_idx;

    controller()
    {
        channel = NULL; // null pointer 
    }
    
    ~controller() {}
    
    virtual void integrate(double, double, double) = 0;
    virtual double get_gbar(void) = 0;
    virtual double get_m(void) = 0;

};





#endif
