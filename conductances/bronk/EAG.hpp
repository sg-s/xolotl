// EAG channels: K+ channels that are inactivated by Calcium.
#ifndef EAG
#define EAG
#include "../../conductance.h"

//inherit conductance class spec
class EAG: public conductance {

public:

    // specify parameters + initial conditions 
    EAG(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }
    
    EAG(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }
    
    void integrate(double V, double Ca, double dt);
    void connect(compartment *pcomp_);
    double m_inf(double V, double Ca);
    double tau_m(double V);
};

void EAG::connect(compartment *pcomp_) {container = pcomp_; }

void EAG::integrate(double V, double Ca, double dt)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V));
    g = gbar*m*m;
}

double EAG::m_inf(double V, double Ca) { return (9.29e-2/(Ca+9.29e-2))/(1.0+exp((V+23.12)/-16.94)); }
double EAG::tau_m(double V) {return 5497 - 5500/(1.0+exp((V+251.5 )/-51.5));}


#endif
