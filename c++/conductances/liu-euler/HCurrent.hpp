// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// H current. again, for mysterious reasons, the compiler
// won't let me call this class "H"
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef HCURRENT
#define HCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {

public:

    //specify both gbar and erev and initial conditions
    HCurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);



};

string HCurrent::getClass(){return "HCurrent";}

void HCurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    m += (m_inf(V) - m)*(dt/tau_m(V));
    g = gbar*m;
}


double HCurrent::m_inf(double V) {return 1.0/(1.0+exp((V+70.0)/6.0));}
double HCurrent::tau_m(double V) {return (272.0 + 1499.0/(1.0+exp((V+42.2)/-8.73)));}

#endif
