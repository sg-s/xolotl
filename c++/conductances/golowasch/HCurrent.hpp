// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// H current fit directly to I-V curve in Golowasch and Marder 1992
// You won't find this anywhere else
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

        // allow this channel to be approximated
        approx_m = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -20; }

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);
};

string HCurrent::getClass(){
    return "HCurrent";
}

double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+95.16)/9.12));}
double HCurrent::tau_m(double V, double Ca) {return (272.0 + 1499.0/(1.0+exp((V+42.2)/-8.73)));}


#endif
