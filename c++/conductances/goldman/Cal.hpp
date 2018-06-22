// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Combined Fast and SLow Calcium Conductance
// http://www.jneurosci.org/content/21/14/5229.long
// Goldman, Golowasch, Marder, Abbott 2012
#ifndef CAL
#define CAL
#include "conductance.hpp"

//inherit conductance class spec
class Cal: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Cal(double g_, double E_, double m_, double h_)
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

string Cal::getClass(){return "Ca";}

void Cal::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}


double Cal::m_inf(double V) {return (1.0/(1.0+exp(((V)+35.0)/-12.0)));}
double Cal::tau_m(double V) {return 7.0+(0.4/(1.0+exp(((V)+70.0)/-15.0)));}
double Cal::h_inf(double V) {return (1.0/(1.0+exp(((V)+40.0)/30.0)));}
double Cal::tau_h(double V) {return 90.0;}


#endif
