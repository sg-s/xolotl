// Slow Calcium conductance
// http://jn.physiology.org/content/jn/90/6/3998.full.pdf
#ifndef KCA
#define KCA
#include "../../conductance.hpp"

//inherit conductance class spec
class KCa: public conductance {

public:

    // specify parameters + initial conditions 
    KCa(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }
    
    KCa(double g_, double E_, double m_)
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

void KCa::connect(compartment *pcomp_) {container = pcomp_; }

void KCa::integrate(double V, double Ca, double dt)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V));
    g = gbar*m*m*m*m;
}

double KCa::m_inf(double V, double Ca) { return (Ca/(Ca+3.0))/(1.0+exp((V+28.3)/-12.6)); }
double KCa::tau_m(double V) {return 180.6 - 150.2/(1.0+exp((V+46.0)/-22.7));}


#endif
