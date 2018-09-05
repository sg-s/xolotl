// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// I_m current
// from 
// Yamada WM, Koch C, Adams P (1989) Multiple channels
// and calcium dynamics In: Methods in neuronal modeling.
// From synapses to networks (Koch C, Segev I, eds),
// pp 97-133. Cambridge, MA: MIT Press.
// 
// also found in 
// https://huguenardlab.stanford.edu/reprints/25_Cereb_Cortex-1993-McCormick-387-98.pdf

#ifndef IM
#define IM
#include "conductance.hpp"

//inherit conductance class spec
class IM: public conductance {

public:

    // specify parameters + initial conditions
    IM(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -100; }
    }

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);


};

string IM::getClass(){return "IM";}

void IM::integrate(double V, double Ca, double delta_temp)
{

    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    g = gbar*m;


}

double IM::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+35.0)/-10));}

double IM::tau_m(double V, double Ca) {return 1e3(3.3*exp((V+35)/20) + exp((V+35)/(-20)));}



#endif
