// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow Calcium conductance
// this version does not support temperature dependence
// http://jn.physiology.org/content/jn/90/6/3998.full.pdf
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

        p = 3;
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

double CaS::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+33.0)/-8.1));}
double CaS::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+60.0)/6.2));}
double CaS::tau_m(double V, double Ca) {return 2.8 + 14.0/(exp((V+27.0)/10.0) + exp((V+70.0)/-13.0));}
double CaS::tau_h(double V, double Ca) {return 120.0 + 300.0/(exp((V+55.0)/9.0) + exp((V+65.0)/-16.0));}


#endif
