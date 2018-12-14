// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Modulatory Input Conductance
// http://www.jneurosci.org/content/34/14/4963/tab-figures-data
// based on RPCH/proctolin recordings
#ifndef MICURRENT
#define MICURRENT
#include "conductance.hpp"

//inherit conductance class spec
class MICurrent: public conductance {

public:


    //specify both gbar and erev and initial conditions
    MICurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 30; }

        p = 1;
        approx_m = 1;
    }

    void integrate(double, double);
    //
    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);
    //

};

string MICurrent::getClass(){return "MICurrent";}



double MICurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+12.0)/-5));}
double MICurrent::tau_m(double V, double Ca) {return 0.5;}

#endif
