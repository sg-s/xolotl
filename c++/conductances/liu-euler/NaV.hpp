// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Sodium CONDUCTANCE
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
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
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);
    double getCurrent(double V, double Ca);

};

string NaV::getClass(){return "NaV";}

void NaV::connect(compartment *pcomp_) {container = pcomp_; }

void NaV::integrate(double V, double Ca, double dt, double delta_temp)
{
    m += (m_inf(V) - m)*(dt/tau_m(V));
    h += (h_inf(V) - h)*(dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double NaV::m_inf(double V) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double NaV::h_inf(double V) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double NaV::tau_m(double V) {return 1.32 - 1.26/(1+exp((V+120.0)/-25.0));}
double NaV::tau_h(double V) {return (0.67/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6)));}

double NaV::getCurrent(double V, double Ca) {return gbar*m*m*m*(V-E);}

#endif
