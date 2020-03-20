// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: WT EAG channels that are inactivated by Calcium.
// component source [Bronk et al. 2017](https://www.physiology.org/doi/10.1152/jn.00820.2017)
//
#ifndef EAGWT
#define EAGWT
#include "conductance.hpp"

//inherit conductance class spec
class EAGwt: public conductance {

public:

    // specify parameters + initial conditions
    EAGwt(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;

    }

    double m_inf(double V, double Ca);
    double tau_m(double, double);
    string getClass(void);

};

string EAGwt::getClass(){return "EAG";}

double EAGwt::m_inf(double V, double Ca) { return (9.29e-2/(Ca+9.29e-2))/(1.0+exp((V+23.12)/-16.94)); }
double EAGwt::tau_m(double V, double Ca) {return 5497 - 5500/(1.0+exp((V+251.5 )/-51.5));}

#endif
