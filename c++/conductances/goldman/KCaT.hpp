// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Transient Calcium-Dependent Potassium Conductance
// http://www.jneurosci.org/content/21/14/5229.long
// Goldman, Golowasch, Marder, Abbott 2012
#ifndef KCAT
#define KCAT
#include "conductance.hpp"

//inherit conductance class spec
class KCaT: public conductance {

public:


    //specify both gbar and erev and initial conditions
    KCaT(double g_, double E_, double m_, double h_)
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

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);

};

string KCaT::getClass(){return "KCaT";}

void KCaT::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*h;
}


double KCaT::m_inf(double V) {return (1.0/(1.0+exp(((V)+10.0)/-1.0)));}
double KCaT::tau_m(double V) {return 43.0+(12.0/(1.0+exp(((V)+10.0)/4.0)));}
double KCaT::h_inf(double V) {return (1.0/(1.0+exp(((V)+36.0)/8.0)));}
double KCaT::tau_h(double V) {return 95.0+(33.0/(1.0+exp(((V)+36.0)/-22.0)));}


#endif
