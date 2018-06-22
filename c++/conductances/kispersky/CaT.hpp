// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Transient Calcium Conductance
// http://www.jneurosci.org/content/32/32/10995
// Kispersky, Caplan, Marder 2012
#ifndef CAT
#define CAT
#include "conductance.hpp"

//inherit conductance class spec
class CaT: public conductance {

public:


    //specify both gbar and erev and initial conditions
    CaT(double g_, double E_, double m_, double h_)
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

string CaT::getClass(){return "CaT";}

void CaT::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}


double CaT::m_inf(double V) {return (1.0/(1.0+exp(((V)+27.1)/-7.2)));}
double CaT::tau_m(double V) {return 21.7-(21.3/(1.0+exp(((V)+68.1)/-20.5)));}
double CaT::h_inf(double V) {return (1.0/(1.0+exp(((V)+32.1)/5.5)));}
double CaT::tau_h(double V) {return 105.0-((89.8)/(1.0+exp(((V)+55.0)/-16.9)));}


#endif
