// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// transient Sodium CONDUCTANCE
// http://www.jneurosci.org/content/32/21/7267
#ifndef NAT
#define NAT
#include "conductance.hpp"

//inherit conductance class spec
class NaT: public conductance {

public:

    // specify parameters + initial conditions
    NaT(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 50; }

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string NaT::getClass(){return "NaT";}


double NaT::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+29.13)/-8.92));}
double NaT::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+40.0)/6.04));}
double NaT::tau_m(double V, double Ca) {return 3.86 - 3.43/(1+exp((V+51.350)/-5.98));}
double NaT::tau_h(double V, double Ca) {return 2.83 - 2.37/(1+exp((V+21.9)/-2.64));}

#endif
