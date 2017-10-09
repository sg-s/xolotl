// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
//Abstract class for defining conductances
#ifndef CONDUCTANCE
#define CONDUCTANCE
#include <cmath>
class compartment;
class controller; 

class conductance {
protected:
    double gbar;
    compartment *container; // pointer to compartment that contains this
public:

    double g;
    double E;
    double m;
    double h;

    conductance()
    {
        container = 0; //null pointer for safety
    }
    
    ~conductance() {}
    
    virtual void integrate(double, double, double) = 0;
    virtual void connect(compartment*) = 0; // null points for safety

};



#endif
