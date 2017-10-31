// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// DmNaV (para) in Drosophila
// http://www.sciencedirect.com/science/article/pii/S0301008200000666
#ifndef DMNAV
#define DMNAV
#include "../../conductance.hpp"

//inherit conductance class spec
class DmNaV: public conductance {

public:

    // specify parameters + initial conditions 
    DmNaV(double g_, double E_, double m_, double h_)
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

void DmNaV::connect(compartment *pcomp_) {container = pcomp_; }

void DmNaV::integrate(double V, double Ca, double dt)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double DmNaV::m_inf(double V) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double DmNaV::h_inf(double V) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double DmNaV::tau_m(double V) {return 1.32 - 1.26/(1+exp((V+120.0)/-25.0));}
double DmNaV::tau_h(double V) {return (0.67/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6)));}

#endif
