// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow Calcium conductance
// http://jn.physiology.org/content/94/1/590.short
#ifndef CAS
#define CAS
#include "conductance.hpp"

//inherit conductance class spec
class CaS: public conductance {

public:

    // specify parameters + initial conditions
    CaS(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        // allow this channel to be approximated
        approx_m = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 50; }

    }

    void integrate(double, double);
    void integrateMS(int,double, double);

    double m_inf(double, double);
    double tau_m(double, double);
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

double CaS::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+22.0)/8.5));}
double CaS::tau_m(double V, double Ca) {return 16 - 13.1/(1 + exp(-(V+25.1)/-26.4));}


#endif
