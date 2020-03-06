// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Sodium CONDUCTANCE
// 
#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions
    NaV(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 50; }

        unitary_conductance = 20e-6; // uS

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 0;
        approx_h = 0;
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);


    double AlphaM(double);
    double AlphaH(double);
    double BetaM(double);
    double BetaH(double);

    string getClass(void);
};

string NaV::getClass(){return "NaV";}


double NaV::AlphaM(double V) {
    return (0.1 * (V+40))/ (1-exp((V+40)/-10));
}

double NaV::AlphaH(double V) {
    return 0.07*exp((V+65)/-20);
}

double NaV::BetaM(double V) {
    return 4*exp((V+65)/-18);
}

double NaV::BetaH(double V) {
    return 1/(1+exp((35+V)/-10));
}



// problematic -- this doesn't really work
double NaV::m_inf(double V, double Ca) {return AlphaM(V)/(AlphaM(V)+BetaM(V));}

double NaV::tau_m(double V, double Ca) {return 1.0/(AlphaM(V)+BetaM(V));}

double NaV::h_inf(double V, double Ca) {return AlphaH(V)/(AlphaH(V)+BetaH(V));}
double NaV::tau_h(double V, double Ca) {return 1.0/(AlphaH(V)+BetaH(V));}



#endif
