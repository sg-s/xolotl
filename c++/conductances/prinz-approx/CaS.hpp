// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow Calcium conductance
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef CAS
#define CAS
#include "conductance.hpp"

//inherit conductance class spec
class CaS: public conductance {

public:

    // specify parameters + initial conditions
    CaS(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults 
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }

        // cache values for m_inf and h_inf
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V);
            h_inf_cache[(int) round(V+99)] = h_inf(V);
            tau_m_cache[(int) round(V+99)] = tau_m(V);
            tau_h_cache[(int) round(V+99)] = tau_h(V);
        }

    }

    double m_inf_cache[200];
    double h_inf_cache[200];
    double tau_m_cache[200];
    double tau_h_cache[200];

    double taum;
    double tauh;
    double minf;
    double hinf;

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);


};

string CaS::getClass(){return "CaS";}

void CaS::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;

    // clamp the voltage inside of cached range
    if (V > 101.0) {V = 101.0;}
    else if (V < -99.0) {V = -99.0;}

    minf = m_inf_cache[(int) round(V+99)];
    hinf = h_inf_cache[(int) round(V+99)];
    taum = tau_m_cache[(int) round(V+99)];
    tauh = tau_h_cache[(int) round(V+99)];


    m = minf + (m - minf)*exp(-(dt/taum));
    h = hinf + (h - hinf)*exp(-(dt/tauh));
    g = gbar*m*m*m*h;

    // compute the specific calcium current and update it in the cell
    double this_I = g*(V-E);
    container->i_Ca += this_I;

}

double CaS::m_inf(double V) {return 1.0/(1.0+exp((V+33.0)/-8.1));}
double CaS::h_inf(double V) {return 1.0/(1.0+exp((V+60.0)/6.2));}
double CaS::tau_m(double V) {return 2.8 + 14.0/(exp((V+27.0)/10.0) + exp((V+70.0)/-13.0));}
double CaS::tau_h(double V) {return 120.0 + 300.0/(exp((V+55.0)/9.0) + exp((V+65.0)/-16.0));}

#endif
