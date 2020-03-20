// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: temperature-sensitive A current (K+)
// component source [Prinz et al. 2003](http://jn.physiology.org/content/jn/90/6/3998.full.pdf)
//
#ifndef ACURRENT
#define ACURRENT
#include "conductance.hpp"

//inherit conductance class spec
class ACurrent: public conductance {

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
    ACurrent(double gbar_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
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
        if (isnan (E)) { E = -80; }

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

string ACurrent::getClass(){return "ACurrent";}

void ACurrent::connect(compartment *pcomp_) {
    // call super class method
    conductance::connect(pcomp_);

    // also set up some useful things
    delta_temp = (temperature - temperature_ref)/10;
    pow_Q_tau_m_delta_temp = 1/(pow(Q_tau_m, delta_temp));
    pow_Q_tau_h_delta_temp = 1/(pow(Q_tau_h, delta_temp));
    pow_Q_g = pow(Q_g, delta_temp);
}

void ACurrent::integrateLangevin(double V, double Ca) {
    conductance::integrateLangevin(V,Ca);
    g = pow_Q_g*g;
}


void ACurrent::integrate(double V, double Ca) {
    conductance::integrate(V,Ca);
    g = pow_Q_g*g;
}

double ACurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+27.2)/-8.7)); }
double ACurrent::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+56.9)/4.9)); }
double ACurrent::tau_m(double V, double Ca) {return pow_Q_tau_m_delta_temp*(23.2 - 20.8/(1.0+exp((V+32.9)/-15.2)));}
double ACurrent::tau_h(double V, double Ca) {return pow_Q_tau_h_delta_temp*(77.2 - 58.4/(1.0+exp((V+38.9)/-26.5)));}


#endif
