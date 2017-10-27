// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// ACurrent in AB
// http://jn.physiology.org/content/94/1/590.short
#ifndef ACURRENTAB
#define ACURRENTAB
#include "../../conductance.hpp"

//inherit conductance class spec
class ACurrentAB: public conductance {

public:

    // specify parameters + initial conditions 
    ACurrentAB(double g_, double E_, double m_, double h_)
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

void ACurrentAB::connect(compartment *pcomp_) {container = pcomp_;}

void ACurrentAB::integrate(double V, double Ca, double dt)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double ACurrentAB::m_inf(double V) {return 1.0/(1.0+exp(-(V+27)/8.7)); }
double ACurrentAB::h_inf(double V) {return 1.0/(1.0+exp((V+56.9)/4.9)); }
double ACurrentAB::tau_m(double V) {return 11.6 - 10.4/(1.0+exp(-(V+32.9)/15.2));}
double ACurrentAB::tau_h(double V) {return 38.6 - 29.2/(1.0+exp(-(V+38.9)/26.5));}


#endif
