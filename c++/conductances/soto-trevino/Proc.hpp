// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Persisent Sodium
// http://jn.physiology.org/content/94/1/590.short

#ifndef PROC
#define PROC
#include "../../conductance.hpp"

//inherit conductance class spec
class Proc: public conductance {
    
public:

    //specify both gbar and erev and initial conditions
    Proc(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }

    Proc(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }

    void integrate(double V, double Ca, double dt);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);

};

void Proc::connect(compartment *pcomp_) {container = pcomp_;}

void Proc::integrate(double V, double Ca, double dt)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/0.5);
    g = gbar*m;
}


double Proc::m_inf(double V) {return 1.0/(1.0+exp(-(V+12.0)/3.05));}



#endif
