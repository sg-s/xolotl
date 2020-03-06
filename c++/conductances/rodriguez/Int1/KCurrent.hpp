// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// inactivating potassium conductance
// Rodriguez et al. 2013
// http://www.jneurosci.org/content/jneuro/33/46/18047.full.pdf

#ifndef KCURRENT
#define KCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class KCurrent: public conductance {

public:

    // specify parameters + initial conditions
    KCurrent(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 4;
        q = 2;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = -70; }
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string KCurrent::getClass(){return "KCurrent";}

double KCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+26.0)/-9.0));}
double KCurrent::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+16.0)/1.5));}
double KCurrent::tau_m(double V, double Ca) {return 27.0;}
double KCurrent::tau_h(double V, double Ca) {return 20.0;}

// NOTE
// reported time constants are in the range (27, 3) and (20, 200) ms respectively.


#endif
