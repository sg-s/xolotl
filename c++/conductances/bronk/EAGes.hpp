// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// EAG channels: K+ channels that are inactivated by Calcium.
// this variant is an extra sensitive version of the channel 
// that is 2 orders more sensitive 
// paper source:
// https://www.physiology.org/doi/10.1152/jn.00820.2017

#ifndef EAGES
#define EAGES
#include "../../conductance.hpp"

//inherit conductance class spec
class EAGes: public conductance {

public:

    // specify parameters + initial conditions 
    EAGes(double g_, double E_, double m_)
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

string EAGes::getClass(){return "EAG";}

void EAGes::connect(compartment *pcomp_) {container = pcomp_; }

void EAGes::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V));
    g = pow(Q_g, delta_temp)*gbar*m*m;
}

double EAGes::m_inf(double V, double Ca) { return (9.29e-4/(Ca+9.29e-4))/(1.0+exp((V+23.12)/-16.94)); }
double EAGes::tau_m(double V) {return 5497 - 5500/(1.0+exp((V+251.5 )/-51.5));}


#endif
