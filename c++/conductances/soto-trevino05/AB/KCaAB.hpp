// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Calcium-dep potassium
// http://jn.physiology.org/content/94/1/590.short
#ifndef KCAAB
#define KCAAB
#include "conductance.hpp"

//inherit conductance class spec
class KCaAB: public conductance {

public:

    // specify parameters + initial conditions
    KCaAB(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        p = 4;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }
    }

    double m_inf(double V, double Ca);
    double tau_m(double, double);
    string getClass(void);
};

string KCaAB::getClass(){return "KCaAB";}

double KCaAB::m_inf(double V, double Ca) { return (Ca/(Ca+30))/(1.0+exp(-(V+51.0)/4)); }
double KCaAB::tau_m(double V, double Ca) {return 90.3 - 75.09/(1.0+exp(-(V+46.0)/22.7));}


#endif
