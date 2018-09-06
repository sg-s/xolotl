// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// persistent Sodium CONDUCTANCE
// http://www.jneurosci.org/content/32/21/7267
#ifndef NAP
#define NAP
#include "conductance.hpp"

//inherit conductance class spec
class NaP: public conductance {

public:

    // specify parameters + initial conditions
    NaP(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 50; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};

string NaP::getClass(){return "NaP";}

void NaP::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt);
    g = gbar*m;
}

double NaP::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+48.77)/-3.68));}

#endif
