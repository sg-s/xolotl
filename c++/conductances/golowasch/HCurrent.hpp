// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// 
// component source: [Golowasch & Marder 1992](https://journals.physiology.org/doi/abs/10.1152/jn.1992.67.2.318)
// component info: H current fit directly to I-V curve in Golowasch and Marder 1992
#ifndef HCURRENT
#define HCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {

public:

    //specify both gbar and erev and initial conditions
    HCurrent(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        p = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -20; }

        name = "HCurrent";

    }

    double m_inf(double, double);
    double tau_m(double, double);
    
};



double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+95.16)/9.12));}

// we don't know what the timescale is, we're guessing it's slow
double HCurrent::tau_m(double V, double Ca) {return 2e3;}


#endif
