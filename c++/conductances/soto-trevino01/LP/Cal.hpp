// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// calcium conductance
// https://www.nature.com/articles/nn0301_297#methods
// Soto-Trevino et al. 2001

#ifndef CAL
#define CAL
#include "conductance.hpp"

//inherit conductance class spec
class Cal: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Cal(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }
    }

    void integrate(double, double);
    void integrateMS(int,double, double);

    double m_inf(double, double);
    string getClass(void);

};

string Cal::getClass(){return "Cal";}

void Cal::integrate(double V, double Ca) {
    E = container->E_Ca;
    conductance::integrate(V,Ca);
    container->i_Ca += getCurrent(V);
}

void Cal::integrateMS(int k, double V, double Ca) {
    E = container->E_Ca;
    conductance::integrateMS(k, V, Ca);
    container->i_Ca += getCurrent(V);
}

double Cal::m_inf(double V, double Ca) {return (0.5)*(1.0+tanh((V)+10.0)/5.0);}

#endif
