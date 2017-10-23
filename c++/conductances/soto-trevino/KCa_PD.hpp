// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Calcium-dep potassium
// http://jn.physiology.org/content/94/1/590.short
#ifndef KCA_PD
#define KCA_PD
#include "../../conductance.hpp"

//inherit conductance class spec
class KCa_PD: public conductance {

public:

    // specify parameters + initial conditions 
    KCa_PD(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }
    
    KCa_PD(double g_, double E_, double m_)
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

void KCa_PD::connect(compartment *pcomp_) {container = pcomp_; }

void KCa_PD::integrate(double V, double Ca, double dt)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V));
    g = gbar*m*m*m*m;
}

double KCa_PD::m_inf(double V, double Ca) { return (Ca/(Ca+30))/(1.0+exp(-(V+51.0)/8.0)); }
double KCa_PD::tau_m(double V) {return 90.3 - 75.09/(1.0+exp(-(V+46.0)/22.7));}


#endif
