// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// transient LTS current
// Rodriguez et al. 2013
// http://www.jneurosci.org/content/jneuro/33/46/18047.full.pdf

#ifndef TRANSLTS
#define TRANSLTS
#include "conductance.hpp"

//inherit conductance class spec
class TransLTS: public conductance {

public:

    // specify parameters + initial conditions
    TransLTS(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 1;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 0; }
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string TransLTS::getClass(){return "TransLTS";}

double TransLTS::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+55.0)/-3.0));}
double TransLTS::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+63.0)/0.8));}
double TransLTS::tau_m(double V, double Ca) {return 500.0;}
double TransLTS::tau_h(double V, double Ca) {return 3500.0;}

// NOTE
// reported time constants are in the range (500, 500) and (3500, 1500) ms respectively.


#endif
