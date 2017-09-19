//Abstract class for defining conductances
#ifndef CONDUCTANCE
#define CONDUCTANCE
#include <cmath>
class compartment;

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
    
    double getGBar(void);
    double getG(void);
    double getGE(void); 
    
    virtual void integrate(double, double, double) = 0;
    virtual void connect(compartment*) = 0; // null points for safety

};


double conductance::getGBar(void)
{
    return gbar;
}


double conductance::getG(void)
{
    return g;
}

double conductance::getGE(void)
{
    return g*E;
}



#endif
