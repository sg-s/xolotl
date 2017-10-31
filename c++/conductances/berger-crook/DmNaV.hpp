// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// DmNaV (Sodium conductance in Drosophila (para))
// https://www.frontiersin.org/articles/10.3389/fncom.2015.00139/full
#ifndef DMNAV
#define DMNAV
#include "../../conductance.hpp"

//inherit conductance class spec
class DmNaV: public conductance {

public:

    // specify parameters + initial conditions 
    DmNaV(double g_, double E_, double m_, double h_)
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

void DmNaV::connect(compartment *pcomp_) {container = pcomp_; }

void DmNaV::integrate(double V, double Ca, double dt)
{

    // 1000kbT = 4.1419e-18
    // zme = 4.8000e-19
    // 

    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double DmNaV::m_inf(double V) {return 1.0/(1.0+exp((V+33)/-8.63));}
double DmNaV::h_inf(double V) {return 1.0/(1.0+exp((V+39.14)/4.9715));}
double DmNaV::tau_m(double V) {return .1;}
double DmNaV::tau_h(double V) {return exp((V+39.14)/-13.08)/(0.2*(1 + exp((V+39.14)/-4.9715)));}

#endif
