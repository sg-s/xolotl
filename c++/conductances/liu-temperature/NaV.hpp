// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Sodium CONDUCTANCE
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {
private: 
    double delta_temp = 0;
    double pow_Q_tau_m_delta_temp = 0;
    double pow_Q_tau_h_delta_temp = 0;
    double pow_Q_g = 0;

public:

    double Q_g;
    double Q_tau_m;
    double Q_tau_h;

    // specify parameters + initial conditions
    NaV(double gbar_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;


        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;
        Q_tau_h = Q_tau_h_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (Q_g)) { Q_g = 1; }
        if (isnan (Q_tau_m)) { Q_tau_m = 2; }
        if (isnan (Q_tau_h)) { Q_tau_h = 2; }
        if (isnan (E)) { E = 50; }

        p = 3;
        q = 1;

    }

    void integrate(double, double);
    void integrateLangevin(double, double);
    void connect(compartment*);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string NaV::getClass(){return "NaV";}

void NaV::connect(compartment *pcomp_) {
    // call super class method
    conductance::connect(pcomp_);

    // also set up some useful things
    delta_temp = (temperature - temperature_ref)/10;
    pow_Q_tau_m_delta_temp = (pow(Q_tau_m, delta_temp));
    pow_Q_tau_h_delta_temp = (pow(Q_tau_h, delta_temp));
    pow_Q_g = pow(Q_g, delta_temp);
}


void NaV::integrateLangevin(double V, double Ca) {
    conductance::integrateLangevin(V,Ca);
    g = pow_Q_g*g;
}


void NaV::integrate(double V, double Ca) {
    conductance::integrate(V,Ca);
    g = pow_Q_g*g;
}

double NaV::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double NaV::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double NaV::tau_m(double V, double Ca) {return pow_Q_tau_m_delta_temp*(1.32 - 1.26/(1+exp((V+120.0)/-25.0)));}
double NaV::tau_h(double V, double Ca) {return pow_Q_tau_h_delta_temp*((0.67/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6))));}

#endif
