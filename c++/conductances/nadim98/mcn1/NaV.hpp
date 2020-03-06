// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Cell MCN1 Fast Sodium Conductance
// http://www.jneurosci.org/content/18/13/5053
#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:


    //specify both gbar and erev and initial conditions
    NaV(double gbar_, double E_, double m_, double h_)
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
        
        
        if (isnan (E)) { E = 0; }

    }


    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};

string NaV::getClass(){return "NaV";}

double NaV::m_inf(double V, double Ca) {return (1.0/(1.0+exp(-0.08*((V)+21.0))));}
double NaV::tau_m(double V, double Ca) {return 0.0;}
double NaV::h_inf(double V, double Ca) {return (1.0/(1.0+exp(0.13*((V)+33.0))));}
double NaV::tau_h(double V, double Ca) {return (5.0/(1.0+exp(-0.12*((V)+62.0))));}


#endif
