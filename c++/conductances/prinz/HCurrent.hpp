// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// H current. again, for mysterious reasons, the compiler
// won't let me call this class "H"
// this version does not support temperature dependence
// http://jn.physiology.org/content/jn/90/6/3998.full.pdf
#ifndef HCURRENT
#define HCURRENT
#include "../../conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {
    
public:

    //specify both gbar and erev and initial conditions
    HCurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -20; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);

};

void HCurrent::connect(compartment *pcomp_) {container = pcomp_;}

void HCurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m;
}


double HCurrent::m_inf(double V) {return 1.0/(1.0+exp((V+75.0)/5.5));}
double HCurrent::tau_m(double V) {return (2/( exp((V+169.7)/(-11.6)) + exp((V- 26.7)/(14.3)) ));}


#endif
