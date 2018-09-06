// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Sodium conductance
// Drion et al 2011
// http://www.ele.uri.edu/faculty/vetter/BME307/Projects/neuron-dopamine-project.pdf
#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions
    NaV(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string NaV::getClass(){return "NaV";}

void NaV::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*h;
}

double NaV::m_inf(double V, double Ca) {return ((-0.025)*(V+40.0))/(exp(-0.1*((V)+40.0))) / ( ((-0.025)*(V+40.0))/(exp(-0.1*((V)+40.0))) + (exp(((V)+65.0)/-18.0)) );}
double NaV::h_inf(double V, double Ca) {return (0.0175)*exp(((V)+65.0)/-20.0))) / ( (0.0175)*exp(((V)+65.0)/-20.0))) + (0.25/(1.0+exp(0.1*((V)+35.0)))) );}
double NaV::tau_m(double V, double Ca) {return 1.0 / ( ((-0.025)*(V+40.0))/(exp(-0.1*((V)+40.0))) + (exp(((V)+65.0)/-18.0)) );}
double NaV::tau_h(double V, double Ca) {return 1.0 / ( (0.0175)*exp(((V)+65.0)/-20.0))) + (0.25/(1.0+exp(0.1*((V)+35.0)))) );}


#endif
