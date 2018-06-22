// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Persistent Potassium Conductance
// http://www.jneurosci.org/content/21/14/5229.long
// Goldman, Golowasch, Marder, Abbott 2012
#ifndef AP
#define AP
#include "conductance.hpp"

//inherit conductance class spec
class Ap: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Ap(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

         // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 0; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};

string Ap::getClass(){return "Ap";}

void Ap::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m;
}


double Ap::m_inf(double V) {return (1.0/(1.0+exp(((V)-12.0)/-11.0)));}
double Ap::tau_m(double V) {return 6.0;}


#endif
