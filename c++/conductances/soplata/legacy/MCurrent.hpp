// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// slow potassium current
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879

#ifndef MCURRENT
#define MCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class MCurrent: public conductance {

public:

    //specify both gbar and erev and initial conditions
    MCurrent(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }

        p = 4;

        // allow this channel to be approximated
        approx_m = 1;
    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

    // auxiliary functions
    double a_m(double, double);
    double b_m(double, double);

};

string MCurrent::getClass(){
    return "MCurrent";
}

double MCurrent::a_m(double V, double Ca) {return 0.0001 * 3.209 * (V + 30) / (1 - exp(-(V+30)/9)); }
double MCurrent::b_m(double V, double Ca) {return -0.0001 * 3.209 * (V + 30) / (1 - exp((V+30)/9)); }
double MCurrent::m_inf(double V, double Ca) {return a_m(V,Ca) / (a_m(V,Ca)+b_m(V,Ca)); }
double MCurrent::tau_m(double V, double Ca) {return 1.0 / (a_m(V,Ca) + b_m(V,Ca)); }

#endif
