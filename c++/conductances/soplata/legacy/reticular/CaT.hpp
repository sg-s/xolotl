// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// T-type calcium conductance
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879

#pragma once


//inherit conductance class spec
class CaT: public conductance {

public:

    // specify parameters + initial conditions
    CaT(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = 30; }

        p = 2;
        q = 1;

        name = "CaT";
        is_calcium = true;

    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    


};


double CaT::m_inf(double V, double Ca) {return 1.0/(1.0+exp((-((V+4.0)+57.0))/6.2)); }
double CaT::h_inf(double V, double Ca) {return 1.0/(1.0+exp(((V+4.0)+81.0)/4.0)); }
double CaT::tau_m(double V, double Ca) {return 0.0; }
double CaT::tau_h(double V, double Ca) {return (30.8+(211.4+exp(((V+4.0)+113.2)/5.0))/(1.0+exp(((V+2.0)+84.0)/3.2)))/3.73; }


