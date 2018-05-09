// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// transient Sodium CONDUCTANCE
// http://www.jneurosci.org/content/32/21/7267
#ifndef NAT
#define NAT
#include "conductance.hpp"

//inherit conductance class spec
class NaT: public conductance {

public:

    // specify parameters + initial conditions
    NaT(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 50; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);
    double getCurrent(double V, double Ca);

};

string NaT::getClass(){return "NaT";}

void NaT::connect(compartment *pcomp_) {container = pcomp_; }

void NaT::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-(dt*pow(Q_tau_h, delta_temp))/tau_h(V));
    g = pow(Q_g, delta_temp)*gbar*m*m*m*h;
}

double NaT::m_inf(double V) {return 1.0/(1.0+exp((V+29.13)/-8.92));}
double NaT::h_inf(double V) {return 1.0/(1.0+exp((V+40.0)/6.04));}
double NaT::tau_m(double V) {return 3.86 - 3.43/(1+exp((V+51.350)/-5.98));}
double NaT::tau_h(double V) {return 2.83 - 2.37/(1+exp((V+21.9)/-2.64));}

double NaT::getCurrent(double V, double Ca) {return gbar*m*m*m*(V-E);}

#endif
