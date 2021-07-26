// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// slow potassium current
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879

#pragma once


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
        name = "MCurrent";

    }

    double m_inf(double, double);
    double tau_m(double, double);
    

    // auxiliary functions
    double a_m(double, double);
    double b_m(double, double);

};


double MCurrent::a_m(double V, double Ca) {return 0.0001 * 3.209 * (V + 30) / (1 - exp(-(V+30)/9)); }
double MCurrent::b_m(double V, double Ca) {return -0.0001 * 3.209 * (V + 30) / (1 - exp((V+30)/9)); }
double MCurrent::m_inf(double V, double Ca) {return a_m(V,Ca) / (a_m(V,Ca)+b_m(V,Ca)); }
double MCurrent::tau_m(double V, double Ca) {return 1.0 / (a_m(V,Ca) + b_m(V,Ca)); }

