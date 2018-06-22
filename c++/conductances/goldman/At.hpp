// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Fast Transient Potassium Conductance
// http://www.jneurosci.org/content/21/14/5229.long
// Goldman, Golowasch, Marder, Abbott 2012
#ifndef AT
#define AT
#include "conductance.hpp"

//inherit conductance class spec
class At: public conductance {

public:


    //specify both gbar and erev and initial conditions
    At(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

         // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 0; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);

};

string At::getClass(){return "At";}

void At::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*h;
}


double At::m_inf(double V) {return (1.0/(1.0+exp(((V)+23.0)/-13.0)));}
double At::tau_m(double V) {return 11.0+(8.0/(1.0+exp(((V)+70.0)/-15.0)));}
double At::h_inf(double V) {return (1.0/(1.0+exp(((V)+62.0)/7.0)));}
double At::tau_h(double V) {return 6.0+(40.0/(1.0+exp(((V)-55.0)/-18.0)));}


#endif
