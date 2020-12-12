// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Inactivating Calcium conductance
// from Turrigiano ... Marder 1995
// http://www.jneurosci.org/content/jneuro/15/5/3640.full.pdf

#ifndef CA1
#define CA1
#include "conductance.hpp"

//inherit conductance class spec
class Ca1: public conductance {

public:

    // specify parameters + initial conditions
    Ca1(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        q = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }        
        if (isnan (E)) { E = 120; }

        name = "Ca1";
        is_calcium = true;

    }
    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    
};



double Ca1::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+27.1)/-7.18));}
double Ca1::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+30.1)/5.5));}
double Ca1::tau_m(double V, double Ca) {return 21.7 - 21.3/(exp((V+68.1)/20.5));}
double Ca1::tau_h(double V, double Ca) {return 105 - 89.8/(exp((V+55.0)/16.9));}

#endif
