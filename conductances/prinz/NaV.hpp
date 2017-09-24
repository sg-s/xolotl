// Sodium CONDUCTANCE
#ifndef NAV
#define NAV
#include "../../conductance.hpp"

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
    }
    
    void integrate(double V, double Ca, double dt);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V); 
};

void NaV::connect(compartment *pcomp_) {container = pcomp_; }

void NaV::integrate(double V, double Ca, double dt)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double NaV::m_inf(double V) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double NaV::h_inf(double V) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double NaV::tau_m(double V) {return 2.64 - 2.52/(1+exp((V+120.0)/-25.0));}
double NaV::tau_h(double V) {return (1.34/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6)));}

#endif
