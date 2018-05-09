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
    Leak(double g_, double E_)
    {
        gbar = g_;
        g = gbar; // this is important as integrate doesn't do anything in the leak channels
        E = E_;

        if (isnan (E)) { E = -55; }

    }



    void integrate(double V, double Ca, double dt, double delta_temp);

    string getClass(void);

};

string Leak::getClass(){return "Leak";}

void Leak::integrate(double V, double Ca, double dt, double delta_temp) {
    // do nothing
}

double Leak::getCurrent(double V, double Ca) {return gbar*(V-E);}


#endif
