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
    Cal(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    string getClass(void);

};

string Cal::getClass(){return "Cal";}

void Cal::integrate(double V, double Ca)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;

    m = m_inf(V,Ca);
    g = gbar*m;

    // compute the specific calcium current and update it in the cell
    double this_I = g*(V-E);
    container->i_Ca += this_I;
}


double Cal::m_inf(double V, double Ca) {return (0.5)*(1.0+tanh((V)+20.0)/8.0);}

#endif
