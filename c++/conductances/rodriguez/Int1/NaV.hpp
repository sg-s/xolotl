// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// fast sodium conductance
// Rodriguez et al. 2013
// http://www.jneurosci.org/content/jneuro/33/46/18047.full.pdf

#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions
    NaV(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 45; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);


};

string NaV::getClass(){return "NaV";}

void NaV::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double NaV::m_inf(double V) {return 1.0/(1.0+exp((V+42.0)/-5.0));}
double NaV::h_inf(double V) {return 1.0/(1.0+exp((V+52.0)/9.2));}
double NaV::tau_m(double V) {return 1.5;}
double NaV::tau_h(double V) {return 10.0;}

// NOTE
// reported time constants are in the range (1.5, 0.45) and (10, 2.4) ms respectively.


#endif
