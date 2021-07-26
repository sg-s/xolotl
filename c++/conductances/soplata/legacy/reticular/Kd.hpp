// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// delayed rectifier potassium conductance
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879

#pragma once


//inherit conductance class spec
class Kd: public conductance {

public:

    //specify both gbar and erev and initial conditions
    Kd(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -80; }

        p = 4;

        name = "Kd";

    }

    double m_inf(double, double);
    double tau_m(double, double);
    

    // auxiliary functions
    double a_m(double, double);
    double b_m(double, double);

};


double Kd::a_m(double V, double Ca) {return 0.032*(15.0-(V+55.0))/(exp((15.0-(V+55.0))/5.0)-1.0); }
double Kd::b_m(double V, double Ca) {return 0.5*exp((10-(V+55.0))/40.0); }
double Kd::m_inf(double V, double Ca) {return a_m(V,Ca) / (a_m(V,Ca)+b_m(V,Ca)); }
double Kd::tau_m(double V, double Ca) {return 1.0 / (a_m(V,Ca) + b_m(V,Ca)); }

