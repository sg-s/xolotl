// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Fast Potassium conductance 
// http://www.jneurosci.org/content/32/21/7267
#ifndef KFAST
#define KFAST
#include "../../conductance.hpp"

//inherit conductance class spec
class Kfast: public conductance {

public:

    // specify parameters + initial conditions 
    Kfast(double g_, double E_, double m_, double h_)
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

void Kfast::connect(compartment *pcomp_) {container = pcomp_; }

void Kfast::integrate(double V, double Ca, double dt)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*m*h; // yeah, paper says p = 4
}

double Kfast::m_inf(double V) {return 1.0/(1.0+exp((V+17.55)/-7.27));}
double Kfast::h_inf(double V) {return 1.0/(1.0+exp((V+45.0)/6.0));}
double Kfast::tau_m(double V) {return 1.94 - 2.66/(1+exp((V-8.12)/7.96));}
double Kfast::tau_h(double V) {return 1.79 - 515.8/(1+exp((V+147.4)/28.66));}

#endif
