// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
//Abstract class for defining controllers 

#ifndef CONTROLLER
#define CONTROLLER
#include <cmath>
class compartment;
class conductance;

class controller {
protected:
    conductance *channel; // pointer to conductance that this regulates 
    compartment *container; // pointer to compartment that contains this
public:

    controller()
    {
        container = 0; //null pointer for safety
        conductance = 0; // null pointer 
    }
    
    ~controller() {}
    
    virtual void integrate(double) = 0;
    virtual void connect(conductance*, compartment*) = 0; 
};
#endif
