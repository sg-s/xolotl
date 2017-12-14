// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Slow Calcium conductance
#ifndef LEAK
#define LEAK
#include "../conductance.hpp"

//inherit conductance class spec
class Leak: public conductance {

public:

    // specify parameters + initial conditions 
    Leak(double g_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
    {
        gbar = g_;
        g = gbar; // this is important as integrate doesn't do anything in the leak channels 
        E = E_;
        m = 1;
        h = 1;
        

        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;
        Q_tau_h = Q_tau_h_;
    }
    


    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
};

void Leak::connect(compartment *pcomp_) {container = pcomp_;}

void Leak::integrate(double V, double Ca, double dt, double delta_temp) {
    // do nothing
}

#endif
