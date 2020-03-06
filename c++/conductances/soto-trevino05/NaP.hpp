// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Persisent Sodium
// http://jn.physiology.org/content/94/1/590.short
#ifndef NAP
#define NAP
#include "conductance.hpp"

//inherit conductance class spec
class NaP: public conductance {

public:

    // specify parameters + initial conditions
    NaP(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        
        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 50; }
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string NaP::getClass(){return "NaP";}

double NaP::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+26.8)/8.2));}
double NaP::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+48.5)/4.8));}
double NaP::tau_m(double V, double Ca) {return 19.8 - 10.7/(1+exp(-(V+26.5)/8.6));}
double NaP::tau_h(double V, double Ca) {return 666 - 379/(1+exp(-(V+33.6)/11.7));}

#endif
