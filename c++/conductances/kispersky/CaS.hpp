// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow Calcium Conductance
// http://www.jneurosci.org/content/32/32/10995
// Kispersky, Caplan, Marder 2012
#ifndef CAS
#define CAS
#include "conductance.hpp"

//inherit conductance class spec
class CaS: public conductance {

public:


    //specify both gbar and erev and initial conditions
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
        if (isnan (E)) { E = 0; }

    }

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};

string CaS::getClass(){return "CaS";}

void CaS::integrate(double V, double Ca, double delta_temp)
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

double CaS::m_inf(double V, double Ca) {return 1.0 / (1.0 + exp( (V + 33.0) / -8.1 ));}
double CaS::h_inf(double V, double Ca) {return 1.0 / (1.0 + exp( (V + 60.0) / 6.2 ));}
double CaS::tau_m(double V, double Ca) {return 1.4 + 7.0 / (exp((V + 27.0)/10) + exp((V + 70.0)/-13.0));}
double CaS::tau_h(double V, double Ca) {return 60.0 + 150.0 / (exp((V + 55.0)/9.0) + exp((V + 65.0)/-16.0));}


#endif
