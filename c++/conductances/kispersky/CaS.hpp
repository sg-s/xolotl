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
    CaS(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 0; }

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

    }

    void integrate(double, double);
    void integrateMS(int, double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};

string CaS::getClass(){return "CaS";}

void CaS::integrate(double V, double Ca) {
    E = container->E_Ca;
    conductance::integrate(V,Ca);
    container->i_Ca += getCurrent(V);
}

void CaS::integrateMS(int k, double V, double Ca) {
    E = container->E_Ca;
    conductance::integrateMS(k, V, Ca);
    container->i_Ca += getCurrent(V);
}


double CaS::m_inf(double V, double Ca) {return 1.0 / (1.0 + exp( (V + 33.0) / -8.1 ));}
double CaS::h_inf(double V, double Ca) {return 1.0 / (1.0 + exp( (V + 60.0) / 6.2 ));}
double CaS::tau_m(double V, double Ca) {return 1.4 + 7.0 / (exp((V + 27.0)/10) + exp((V + 70.0)/-13.0));}
double CaS::tau_h(double V, double Ca) {return 60.0 + 150.0 / (exp((V + 55.0)/9.0) + exp((V + 65.0)/-16.0));}


#endif
