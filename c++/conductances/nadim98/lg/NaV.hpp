// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Cell Int1 Fast Sodium Conductance
// http://www.jneurosci.org/content/18/13/5053
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

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);

};

string NaV::getClass(){return "NaV";}

void NaV::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}


double NaV::m_inf(double V) {return (1.0/(1.0+exp(-0.08*((V)+21.0))));}
double NaV::tau_m(double V) {return 0.0;}
double NaV::h_inf(double V) {return (1.0/(1.0+exp(0.13*((V)+33.0))));}
double NaV::tau_h(double V) {return (5.0/(1.0+exp(-0.12*((V)+62.0))));}


#endif
