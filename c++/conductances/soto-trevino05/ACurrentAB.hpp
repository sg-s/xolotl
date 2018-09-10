// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// ACurrent in AB
// http://jn.physiology.org/content/94/1/590.short
#ifndef ACURRENTAB
#define ACURRENTAB
#include "conductance.hpp"

//inherit conductance class spec
class ACurrentAB: public conductance {

public:

    // specify parameters + initial conditions
    ACurrentAB(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

         // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = -80; }

    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string ACurrentAB::getClass(){return "ACurrentAB";}

void ACurrentAB::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*h;
}

double ACurrentAB::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+27)/8.7)); }
double ACurrentAB::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+56.9)/4.9)); }
double ACurrentAB::tau_m(double V, double Ca) {return 11.6 - 10.4/(1.0+exp(-(V+32.9)/15.2));}
double ACurrentAB::tau_h(double V, double Ca) {return 38.6 - 29.2/(1.0+exp(-(V+38.9)/26.5));}


#endif
