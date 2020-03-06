// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Fast Sodium Conductance
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4406621/
// Jochems & Yoshida 2015
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

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = 50; }

        p = 3;
        h = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    double a_m(double, double);
    double b_m(double, double);
    double a_h(double, double);
    double b_h(double, double);
    string getClass(void);

};

string NaV::getClass(){return "NaV";}


double NaV::a_m(double V, double Ca) {return 0.32 * (13 - V) / (exp((13 - V)/4) - 1);}
double NaV::a_h(double V, double Ca) {return 0.128 * exp((V - 17)/18);}
double NaV::b_m(double V, double Ca) {return 0.28 * (V - 40) / (exp((V - 40)/5) - 1);}
double NaV::b_h(double V, double Ca) {return 4 / (exp(-(V - 40)/5) + 1);}

double NaV::m_inf(double V, double Ca) {return a_m(V, Ca) / (a_m(V, Ca) + b_m(V, Ca));}
double NaV::h_inf(double V, double Ca) {return a_h(V, Ca) / (a_h(V, Ca) + b_h(V, Ca));}
double NaV::tau_m(double V, double Ca) {return 1 / (a_m(V, Ca) + b_m(V, Ca));}
double NaV::tau_h(double V, double Ca) {return 1 / (a_h(V, Ca) + b_h(V, Ca));}

#endif
