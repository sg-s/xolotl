// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Potassium Conductance
// https://www.biorxiv.org/content/biorxiv/early/2018/06/13/344325.full.pdf
// Francisco J. H. Heras, Mikko Vahasoyrinki, and Jeremy E. Niven 2018
#ifndef SHAKER
#define SHAKER
#include "conductance.hpp"

//inherit conductance class spec
class Shaker: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Shaker(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = -80; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);

};

string Shaker::getClass(){return "Shaker";}

void Shaker::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = 0.13 + (1 - 0.13) * (h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V)));
    g = gbar*m*m*m*h;
}


double Shaker::m_inf(double V) {return (1.0/(1.0+exp(((V)+23.7)/-12.8)))^(1.0/3.0);}
double Shaker::tau_m(double V) {return 1.0/(0.00817*exp((-V+1.62)/24.7)+0.0581*(-V-56.6)/(exp((-V-59.6)/4.50))-1.0);}
double Shaker::h_inf(double V) {return (0.8/(1.0+exp((55.3+V)/3.9)))+(0.2/(1.0+exp((74.8+V)/10.7)));}
double Shaker::tau_h(double V) {return 1.0/(0.230*exp((-V-193.0)/31.3)+0.0437*(-V+13.5)/exp((-V-13.5)/11.1));}


#endif
