// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// EAG channels: K+ channels that are inactivated by Calcium.
// paper source:
// https://www.physiology.org/doi/10.1152/jn.00820.2017
#ifndef EAGWT
#define EAGWT
#include "conductance.hpp"

//inherit conductance class spec
class EAGwt: public conductance {

public:

    // specify parameters + initial conditions 
    EAGwt(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }

    }
    

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V, double Ca);
    double tau_m(double V);
    string getClass(void);
};

string EAGwt::getClass(){return "EAG";}

void EAGwt::connect(compartment *pcomp_) {container = pcomp_; }

void EAGwt::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V));
    g = gbar*m*m;
}

double EAGwt::m_inf(double V, double Ca) { return (9.29e-2/(Ca+9.29e-2))/(1.0+exp((V+23.12)/-16.94)); }
double EAGwt::tau_m(double V) {return 5497 - 5500/(1.0+exp((V+251.5 )/-51.5));}


#endif
