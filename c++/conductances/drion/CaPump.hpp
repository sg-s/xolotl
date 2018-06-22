// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Drion et al 2011
// Calcium pump current
// http://www.ele.uri.edu/faculty/vetter/BME307/Projects/neuron-dopamine-project.pdf

#ifndef CAPUMP
#define CAPUMP
#include "conductance.hpp"

//inherit conductance class spec
class CaPump: public conductance {

public:

    //specify both gbar and erev and initial conditions
    CaPump(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
 if (isnan(gbar)) { gbar = 0.0156; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 0; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    string getClass(void);


};

string CaPump::getClass(){
    return "CaPump";
}

void CaPump::integrate(double V, double Ca, double dt, double delta_temp)
{
    g = (gbar)/((1.0+0.0001)/Ca);
}

#endif
