// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Deleyed rectifier
// Dethier, Drion, Franci, Sepulchre 2015
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4620135/

#ifndef HCURRENT
#define HCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {

public:

    // specify parameters + initial conditions
    HCurrent(double g_, double E_, double m_)
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

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);
};

string HCurrent::getClass(){return "HCurrent";}


double HCurrent::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+80.0)/6.0)));}
double HCurrent::tau_m(double V, double Ca) {return 272.0- (1499.0/(1.0+exp(((V)+42.2)/-8.73)));}

#endif
