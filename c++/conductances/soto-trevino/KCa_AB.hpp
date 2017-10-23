// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Calcium-dep potassium
// http://jn.physiology.org/content/94/1/590.short
#ifndef KCA_AB
#define KCA_AB
#include "../../conductance.hpp"

//inherit conductance class spec
class KCa_AB: public conductance {

public:

    // specify parameters + initial conditions 
    KCa_AB(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }
    
    KCa_AB(double g_, double E_, double m_)
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

void KCa_AB::connect(compartment *pcomp_) {container = pcomp_; }

void KCa_AB::integrate(double V, double Ca, double dt)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V));
    g = gbar*m*m*m*m;
}

double KCa_AB::m_inf(double V, double Ca) { return (Ca/(Ca+30))/(1.0+exp(-(V+51.0)/4)); }
double KCa_AB::tau_m(double V) {return 90.3 - 75.09/(1.0+exp(-(V+46.0)/22.7));}


#endif
