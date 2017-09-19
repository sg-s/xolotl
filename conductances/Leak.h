// Slow Calcium conductance
#ifndef LEAK
#define LEAK
#include "../conductance.h"

//inherit conductance class spec
class Leak: public conductance {

public:

    // specify parameters + initial conditions 
    Leak(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = 1;
        h = 1;
        g = gbar;
    }
    
    // specify parameters + initial conditions 
    Leak(double g_, double E_)
    {
        gbar = g_;
        E = E_;
        m = 1;
        h = 1;
        g = gbar;
    }

    void integrate(double V, double Ca, double dt);
    void connect(compartment *pcomp_);
};

void Leak::connect(compartment *pcomp_) {container = pcomp_;}

void Leak::integrate(double V, double Ca, double dt) {
    // do nothing
}

#endif
