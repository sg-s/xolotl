// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// persistent Sodium CONDUCTANCE
// http://www.jneurosci.org/content/32/21/7267
#ifndef NAP
#define NAP
#include "../../conductance.hpp"

//inherit conductance class spec
class NaP: public conductance {

public:

    // specify parameters + initial conditions 
    NaP(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }
    
    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V); 
};

void NaP::connect(compartment *pcomp_) {container = pcomp_; }

void NaP::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt);
    g = pow(Q_g, delta_temp)*gbar*m;
}

double NaP::m_inf(double V) {return 1.0/(1.0+exp((V+48.77)/-3.68));}

#endif
