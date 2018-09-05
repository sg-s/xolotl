// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Potassium Conductance
// http://www.jneurosci.org/content/32/32/10995
// Kispersky, Caplan, Marder 2012
#ifndef ACURRENT
#define ACURRENT
#include "conductance.hpp"

//inherit conductance class spec
class ACurrent: public conductance {

public:


    //specify both gbar and erev and initial conditions
    ACurrent(double g_, double E_, double m_, double h_)
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

string ACurrent::getClass(){return "ACurrent";}

void ACurrent::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}


double ACurrent::m_inf(double V) {return (1.0/(1.0+exp(((V)+27.2)/-8.7)));}
double ACurrent::tau_m(double V) {return 11.6-(10.4/(exp(((V)+32.9)/-15.2)));}
double ACurrent::h_inf(double V) {return (1.0/(1.0+exp(((V)+56.9)/4.9)));}
double ACurrent::tau_h(double V) {return 38.6-((29.2)/(exp(((V)+38.9)/-26.5)));}


#endif
