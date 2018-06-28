// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Drion et al 2011
// L-type calcium channel
// http://www.ele.uri.edu/faculty/vetter/BME307/Projects/neuron-dopamine-project.pdf

#ifndef CAL
#define CAL
#include "conductance.hpp"

//inherit conductance class spec
class CaL: public conductance {

public:

    //specify both gbar and erev and initial conditions
    CaL(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -20; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);


};

string CaL::getClass(){
    return "CaL";
}

void CaL::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;
    
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*(d)*0.00018/(0.00018 + Ca);

    // compute the specific calcium current and update it in the cell
    double this_I = g*(V-E);
    container->i_Ca += this_I;
}


double CaL::m_inf(double V) {return (1.0/(1.0+exp(((V)+55.0)/-3.0)));}
double CaL::tau_m(double V) {return (72.0)*exp((((V)+120.0)^(2))/-400.0)+6.0;}

#endif
