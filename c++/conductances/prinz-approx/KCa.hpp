// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow Calcium conductance
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
// this version uses an approximation of the timescale
// dependence on V for faster code execution

#ifndef KCA
#define KCA
#include "conductance.hpp"

//inherit conductance class spec
class KCa: public conductance {

public:

    // specify parameters + initial conditions
    KCa(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }

        // cache values for m_inf and h_inf
        for (double V = -99; V < 101; V++) {
            tau_m_cache[(int) round(V+99)] = tau_m(V);
        }
    }

    double tau_m_cache[200];
    double taum;

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V, double Ca);
    double tau_m(double V);
    string getClass(void);
};

string KCa::getClass(){return "KCa";}

void KCa::integrate(double V, double Ca, double dt, double delta_temp)
{
    taum = tau_m_cache[(int) round(V+99)];
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/taum);
    g = gbar*m*m*m*m;
}

double KCa::m_inf(double V, double Ca) { return (Ca/(Ca+3.0))/(1.0+exp((V+28.3)/-12.6)); }
double KCa::tau_m(double V) {return 180.6 - 150.2/(1.0+exp((V+46.0)/-22.7));}

#endif
