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
    HCurrent(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = -20; }

        // cache values for m_inf and h_inf
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V);
            tau_m_cache[(int) round(V+99)] = tau_m(V);
        }

    }


    double m_inf_cache[200];
    double tau_m_cache[200];

    double taum;
    double minf;

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};

string HCurrent::getClass(){return "HCurrent";}

void HCurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    minf = m_inf_cache[(int) round(V+99)];
    taum = tau_m_cache[(int) round(V+99)];

    m = minf + (m - minf)*exp(-(dt/taum));
    g = gbar*m*h;
}


double HCurrent::m_inf(double V) {return 1.0/(1.0+exp((V+75.0)/5.5));}
double HCurrent::tau_m(double V) {return (2/( exp((V+169.7)/(-11.6)) + exp((V- 26.7)/(14.3)) ));}


#endif
