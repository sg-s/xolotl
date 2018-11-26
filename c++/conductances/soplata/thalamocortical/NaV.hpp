// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// fast sodium conductance for thalamocortical relay cells
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879

#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions
    NaV(double g_, double E_, double m_, double h_)
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
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

    // auxiliary functions
    double a_m(double, double);
    double b_m(double, double);
    double a_h(double, double);
    double b_h(double, double);

};

string NaV::getClass(){return "NaV";}

double NaV::a_m(double V, double Ca) {return 0.32*(13.0-(V+35.0))/(exp((13.0-(V+35.0))/4.0)-1.0); }
double NaV::b_m(double V, double Ca) {return 0.28*((V+35.0)-40)/(exp(((V+35.0)-40.0)/5.0)-1.0); }
double NaV::a_h(double V, double Ca) {return 0.128*exp((17.0-(V+35.0))/18.0); }
double NaV::b_h(double V, double Ca) {return 4.0/(1.0+exp((40.0-(V+35.0))/5.0)); }

double NaV::m_inf(double V, double Ca) {return a_m(V,Ca) / (a_m(V,Ca) + b_m(V,Ca)); }
double NaV::h_inf(double V, double Ca) {return a_h(V,Ca) / (a_h(V,Ca) + b_h(V,Ca)); }
double NaV::tau_m(double V, double Ca) {return 1.0 / (a_m(V,Ca) + b_m(V,Ca)); }
double NaV::tau_h(double V, double Ca) {return 1.0 / (a_h(V,Ca) + b_h(V,Ca)); }


#endif
