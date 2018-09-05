// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Modulatory Input Conductance
// http://www.jneurosci.org.resources.library.brandeis.edu/content/jneuro/21/11/4050.full.pdf
// based on RPCH/proctolin recordings in Swensen & Marder 2001
#ifndef MICURRENT
#define MICURRENT
#include "conductance.hpp"

//inherit conductance class spec
class MICurrent: public conductance {

public:


    //specify both gbar and erev and initial conditions
    MICurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

         // defaults 
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 0; }

    }

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string MICurrent::getClass(){return "MICurrent";}

void MICurrent::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    g = gbar*m;
}


double MICurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+21.0)/-8));}
double MICurrent::tau_m(double V, double Ca) {return 6.0;}


#endif
