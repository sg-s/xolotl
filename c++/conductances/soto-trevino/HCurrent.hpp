// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// HCurrent
// http://jn.physiology.org/content/94/1/590.short
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
        h = 1;
    }

    HCurrent(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;
    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);

};

void HCurrent::connect(compartment *pcomp_) {container = pcomp_;}

void HCurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V));
    g = pow(Q_g, delta_temp)*gbar*m;
}


double HCurrent::m_inf(double V) {return 1.0/(1.0+exp((V+70.0)/6.0));}
double HCurrent::tau_m(double V) {return (272.0 + 1499.0/(1.0+exp((V+42.2)/-8.73)));}


#endif
