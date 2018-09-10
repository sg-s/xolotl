// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Fast Sodium Conductance
// http://www.jneurosci.org/content/32/32/10995
// Kispersky, Caplan, Marder 2012
#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:


    //specify both gbar and erev and initial conditions
    NaV(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

         // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 0; }

    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};

string NaV::getClass(){return "NaV";}

void NaV::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*h;
}


double NaV::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+25.5)/-5.29)));}
double NaV::tau_m(double V, double Ca) {return 1.32-(1.26/(1.0+exp(((V)+120.0)/-25.0)));}
double NaV::h_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+48.9)/5.18)));}
double NaV::tau_h(double V, double Ca) {return ((0.67)/(1.0+exp(((V)+38.9)/-26.5)))*(1.5+(1.0/(1.0+exp(((V)+34.9)/3.6))));}


#endif
