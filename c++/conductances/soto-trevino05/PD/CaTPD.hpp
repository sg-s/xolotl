// Fast Calcium CONDUCTANCE
// http://jn.physiology.org/content/94/1/590.short
#ifndef CATPD
#define CATPD
#include "conductance.hpp"

//inherit conductance class spec
class CaTPD: public conductance {

public:

    // specify parameters + initial conditions
    CaTPD(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
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
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 50; }

    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string CaTPD::getClass(){return "CaTPD";}

void CaTPD::integrate(double V, double Ca) {
    E = container->E_Ca;
    conductance::integrate(V,Ca);
    container->i_Ca += getCurrent(V);
}

void CaTPD::integrateMS(int k, double V, double Ca) {
    E = container->E_Ca;
    conductance::integrateMS(k, V, Ca);
    container->i_Ca += getCurrent(V);
}

double CaTPD::m_inf(double V, double Ca) {return 1.0/(1.0 + exp((V+25)/-7.2));}
double CaTPD::h_inf(double V, double Ca) {return 1.0/(1.0 + exp((V+36)/7));}
double CaTPD::tau_m(double V, double Ca) {return 55 - 49.5/(1.0 + exp(-(V+58)/17));}
double CaTPD::tau_h(double V, double Ca) {return 350 - 300/(1.0 + exp(-(V+50.0)/16.9));}

#endif
