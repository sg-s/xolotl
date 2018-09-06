// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Drosophila NaV
// Voltage-clamp analysis of sodium channels in wild-type and mutant Drosophila neurons (1988)
// https://www.ncbi.nlm.nih.gov/pubmed/2848103
// assumes a reversal potential of +47mV

#ifndef DMNAV
#define DMNAV
#include "conductance.hpp"

//inherit conductance class spec
class DmNaV: public conductance {

public:

    // specify parameters + initial conditions
    DmNaV(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

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

string DmNaV::getClass(){return "DmNaV";}

void DmNaV::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-(dt*pow(Q_tau_h, delta_temp))/tau_h(V,Ca));
    g = pow(Q_g, delta_temp)*gbar*m*m*m*m*m*h;
}

double DmNaV::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+34.3)/-8.79));}
double DmNaV::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+48)/6));}
double DmNaV::tau_m(double V, double Ca) {return (3 - 2.4/(1+exp((V+33.6)/-9.0)))/5;}
double DmNaV::tau_h(double V, double Ca) {return 3 - 2.53/(1+exp((V+22.8)/-3.5));}

#endif
