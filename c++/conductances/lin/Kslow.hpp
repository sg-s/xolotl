// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// transient Sodium CONDUCTANCE
// http://www.jneurosci.org/content/32/21/7267
#ifndef KSLOW   
#define KSLOW   
#include "../../conductance.hpp"

//inherit conductance class spec
class Kslow: public conductance {

public:

    // specify parameters + initial conditions 
    Kslow(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;
    }
    
    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};

string Kslow::getClass(){return "Kslow";}

void Kslow::connect(compartment *pcomp_) {container = pcomp_; }

void Kslow::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m*m*m*m;
}

double Kslow::m_inf(double V) {return 1.0/(1.0+exp((V+12.85)/-19.91));}
double Kslow::tau_m(double V) {return 2.03 - 1.96/(1+exp((V-29.83)/3.32));}

#endif
