// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// EAG channels: K+ channels that are inactivated by Calcium.
// mutant, less sensitive to Ca 
// paper source:
// https://www.physiology.org/doi/10.1152/jn.00820.2017

#ifndef EAGMUT
#define EAGMUT
#include "../../conductance.hpp"

//inherit conductance class spec
class EAGmut: public conductance {

public:

    // specify parameters + initial conditions 
    EAGmut(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;
    }
    

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V, double Ca);
    double tau_m(double V);
    string getClass(void);
};

string EAGmut::getClass(){return "EAG";}

void EAGmut::connect(compartment *pcomp_) {container = pcomp_; }

void EAGmut::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V));
    g = pow(Q_g, delta_temp)*gbar*m*m;
}

double EAGmut::m_inf(double V, double Ca) { return (.92*(1-.05)/(Ca+.92) + .05)/(1.0+exp((V+23.12)/-16.94)); }
double EAGmut::tau_m(double V) {return 5497 - 5500/(1.0+exp((V+251.5 )/-51.5));}


#endif
