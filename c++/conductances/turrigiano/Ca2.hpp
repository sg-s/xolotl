// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Inactivating Calcium conductance
// from Turrigiano ... Marder 1995
// http://www.jneurosci.org/content/jneuro/15/5/3640.full.pdf

#ifndef CA2
#define CA2
#include "conductance.hpp"

//inherit conductance class spec
class Ca2: public conductance {

public:

    // specify parameters + initial conditions
    Ca2(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 120; }

    }

    void integrate(double, double);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);
};

string Ca2::getClass(){
    return "Ca2";
}

void Ca2::integrate(double V, double Ca) {
    E = container->E_Ca;
    conductance::integrate(V,Ca);
    container->i_Ca += getCurrent(V);
}

void Ca2::integrateMS(int k, double V, double Ca) {
    E = container->E_Ca;
    conductance::integrateMS(k, V, Ca);
    container->i_Ca += getCurrent(V);
}

double Ca2::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+21.6)/-8.5));}
double Ca2::tau_m(double V, double Ca) {return 16 - 13.1/(exp((V+25.1)/-26.4));}

#endif
