// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// transient LTS current
// Rodriguez et al. 2013
// http://www.jneurosci.org/content/jneuro/33/46/18047.full.pdf

#ifndef TRANSLTS
#define TRANSLTS
#include "conductance.hpp"

//inherit conductance class spec
class TransLTS: public conductance {

public:

    // specify parameters + initial conditions
    TransLTS(double g_, double E_, double m_, double h_)
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

string TransLTS::getClass(){return "TransLTS";}

void TransLTS::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*h;
}

double TransLTS::m_inf(double V) {return 1.0/(1.0+exp((V+55.0)/-3.0));}
double TransLTS::h_inf(double V) {return 1.0/(1.0+exp((V+63.0)/0.8));}
double TransLTS::tau_m(double V) {return 500.0;}
double TransLTS::tau_h(double V) {return 3500.0;}

// NOTE
// reported time constants are in the range (500, 500) and (3500, 1500) ms respectively.


#endif
