// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// potassium conductance
// https://www.nature.com/articles/nn0301_297#methods
// Soto-Trevino et al. 2001

#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Kd(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

         // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};

string Kd::getClass(){return "Kd";}

void Kd::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m;
}


double Kd::m_inf(double V) {return (0.5)*(1.0+tanh((V)+20.0)/8.0)));}
double Kd::tau_m(double V) {return 1.0 / ( 0.0008*cosh((V + 5.0)/40.0) );}


#endif
