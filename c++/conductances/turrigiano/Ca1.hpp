// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Inactivating Calcium conductance
// from Turrigiano ... Marder 1995
// http://www.jneurosci.org/content/jneuro/15/5/3640.full.pdf

#ifndef CA1
#define CA1
#include "conductance.hpp"

//inherit conductance class spec
class Ca1: public conductance {

public:

    // specify parameters + initial conditions
    Ca1(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults 
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 120; }

    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string Ca1::getClass(){
    return "Ca1";
}

void Ca1::integrate(double V, double Ca)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*h;

    // compute the specific calcium current and update it in the cell
    double this_I = g*(V-E);
    container->i_Ca += this_I;

}

double Ca1::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+27.1)/-7.18));}
double Ca1::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+30.1)/5.5));}
double Ca1::tau_m(double V, double Ca) {return 21.7 - 21.3/(exp((V+68.1)/20.5));}
double Ca1::tau_h(double V, double Ca) {return 105 - 89.8/(exp((V+55.0)/16.9));}

#endif
