// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// inactivating potassium conductance
// Rodriguez et al. 2013
// http://www.jneurosci.org/content/jneuro/33/46/18047.full.pdf

#ifndef KCURRENT
#define KCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class KCurrent: public conductance {

public:

    // specify parameters + initial conditions
    KCurrent(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = -70; }
    }

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);


};

string KCurrent::getClass(){return "KCurrent";}

void KCurrent::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*m*h*h;
}

double KCurrent::m_inf(double V) {return 1.0/(1.0+exp((V+26.0)/-9.0));}
double KCurrent::h_inf(double V) {return 1.0/(1.0+exp((V+16.0)/1.5));}
double KCurrent::tau_m(double V) {return 27.0;}
double KCurrent::tau_h(double V) {return 20.0;}

// NOTE
// reported time constants are in the range (27, 3) and (20, 200) ms respectively.


#endif
