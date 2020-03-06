// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Fast Calcium CONDUCTANCE
// http://jn.physiology.org/content/94/1/590.short
#ifndef CATAB
#define CATAB
#include "conductance.hpp"

//inherit conductance class spec
class CaTAB: public conductance {

public:

    // specify parameters + initial conditions
    CaTAB(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 50; }

    }

    void integrate(double, double);
    void integrateMS(int,double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string CaTAB::getClass(){return "CaTAB";}

void CaTAB::integrate(double V, double Ca) {
    E = container->E_Ca;
    conductance::integrate(V,Ca);
    container->i_Ca += getCurrent(V);
}

void CaTAB::integrateMS(int k, double V, double Ca) {
    E = container->E_Ca;
    conductance::integrateMS(k, V, Ca);
    container->i_Ca += getCurrent(V);
}

double CaTAB::m_inf(double V, double Ca) {return 1.0/(1.0 + exp((V+25)/-7.2));}
double CaTAB::h_inf(double V, double Ca) {return 1.0/(1.0 + exp((V+36)/7));}
double CaTAB::tau_m(double V, double Ca) {return 55 - 49.5/(1.0 + exp(-(V+58)/17));}
double CaTAB::tau_h(double V, double Ca) {return 87.5 - 75/(1.0 + exp(-(V+50.0)/16.9));}

#endif
