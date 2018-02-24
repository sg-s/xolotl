// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Sodium CONDUCTANCE
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef NAV
#define NAV
#include "../../conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions 
    NaV(double g_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;
        

        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;
        Q_tau_h = Q_tau_h_;


        // cache values for m_inf and h_inf
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V);
            h_inf_cache[(int) round(V+99)] = h_inf(V);
            tau_m_cache[(int) round(V+99)] = tau_m(V);
            tau_h_cache[(int) round(V+99)] = tau_h(V);
        }

    }

    double m_inf_cache[200];
    double h_inf_cache[200];
    double tau_m_cache[200];
    double tau_h_cache[200];

    double taum;
    double tauh;
    double minf;
    double hinf;
    
    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V); 
};

void NaV::connect(compartment *pcomp_) {container = pcomp_; }

void NaV::integrate(double V, double Ca, double dt, double delta_temp)
{
    minf = m_inf_cache[(int) round(V+99)];
    hinf = h_inf_cache[(int) round(V+99)];
    taum = tau_m_cache[(int) round(V+99)];
    tauh = tau_h_cache[(int) round(V+99)];

    m = minf + (m - minf)*exp(-(dt/taum));
    h = hinf + (h - hinf)*exp(-(dt/tauh));
    g = gbar*m*m*m*h;
}

double NaV::m_inf(double V) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double NaV::h_inf(double V) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double NaV::tau_m(double V) {return 2.64 - 2.52/(1+exp((V+120.0)/-25.0));}
double NaV::tau_h(double V) {return (1.34/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6)));}

#endif
