// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
//Abstract class for defining controllers 

#ifndef CONTROLLER
#define CONTROLLER
#include <cmath>
class conductance;

class controller {
protected:
    conductance* channel; // pointer to conductance that this regulates 
    double tau_m; 
    double tau_g; 
    double G; 
    
public:

    double m; 

    controller()
    {
        channel = 0; // null pointer 
    }
    
    ~controller() {}
    
    virtual void integrate(double, double) = 0;

};





#endif
