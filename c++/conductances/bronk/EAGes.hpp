// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// EAG channels: K+ channels that are inactivated by Calcium.
// this variant is an extra sensitive version of the channel
// that is 2 orders more sensitive
// paper source:
// https://www.physiology.org/doi/10.1152/jn.00820.2017

#ifndef EAGES
#define EAGES
#include "conductance.hpp"

//inherit conductance class spec
class EAGes: public conductance {

public:

    // specify parameters + initial conditions
    EAGes(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;
    }

    double m_inf(double V, double Ca);
    double tau_m(double, double);
    string getClass(void);

};

string EAGes::getClass(){return "EAG";}


double EAGes::m_inf(double V, double Ca) { return (9.29e-4/(Ca+9.29e-4))/(1.0+exp((V+23.12)/-16.94)); }
double EAGes::tau_m(double V, double Ca) {return 5497 - 5500/(1.0+exp((V+251.5 )/-51.5));}

#endif
