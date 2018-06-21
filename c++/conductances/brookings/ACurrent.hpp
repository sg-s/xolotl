// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// the A current, a potassium current
// Brookings, Goeritz, Marder 2014
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4274917/?report=printable#app1
#ifndef ACURRENT
#define ACURRENT
#include "conductance.hpp"

//inherit conductance class spec
class ACurrent: public conductance {

public:

    // specify parameters + initial conditions
    ACurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};


string ACurrent::getClass(){
    return "ACurrent";
}

void ACurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m*m*m;
}

double ACurrent::m_inf(double V) {return 1.0 / (1.0 + exp(- (V + 27.2) / (8.7) ) ); }
double ACurrent::tau_m(double V) {return 272 + 1499 * (1 + exp(- (V + 42.2) / (8.73) ) ); }

#endif
