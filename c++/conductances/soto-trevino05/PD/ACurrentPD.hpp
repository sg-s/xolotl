// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// ACurrent in PD
// http://jn.physiology.org/content/94/1/590.short
#ifndef ACURRENTPD
#define ACURRENTPD
#include "conductance.hpp"

//inherit conductance class spec
class ACurrentPD: public conductance {

public:

    // specify parameters + initial conditions
    ACurrentPD(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = -80; }

    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string ACurrentPD::getClass(){return "ACurrentPD";}

double ACurrentPD::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+27)/8.7)); }
double ACurrentPD::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+56.9)/4.9)); }
double ACurrentPD::tau_m(double V, double Ca) {return 11.6 - 10.4/(1.0+exp(-(V+32.9)/15.2));}
double ACurrentPD::tau_h(double V, double Ca) {return 38.6 - 29.2/(1.0+exp(-(V+38.9)/26.5));}


#endif
