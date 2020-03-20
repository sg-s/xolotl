// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: temperature-sensitive slow Calcium current
// component source [Prinz et al. 2003](http://jn.physiology.org/content/jn/90/6/3998.full.pdf)
//
#ifndef CAS
#define CAS
#include "conductance.hpp"

//inherit conductance class spec
class CaS: public conductance {

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
    CaS(double gbar_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
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
        if (isnan (E)) { E = 30; }

        is_calcium = true;
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

string CaS::getClass(){return "CaS";}

void CaS::connect(compartment *pcomp_) {
    // call super class method
    conductance::connect(pcomp_);

    // also set up some useful things
    delta_temp = (temperature - temperature_ref)/10;
    pow_Q_tau_m_delta_temp = 1/(pow(Q_tau_m, delta_temp));
    pow_Q_tau_h_delta_temp = 1/(pow(Q_tau_h, delta_temp));
    pow_Q_g = pow(Q_g, delta_temp);
}

void CaS::integrate(double V, double Ca) {
    conductance::integrate(V,Ca);
    g = pow_Q_g*g;
}

void CaS::integrateLangevin(double V, double Ca) {
    conductance::integrateLangevin(V,Ca);
    g = pow_Q_g*g;
}


double CaS::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+33.0)/-8.1));}
double CaS::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+60.0)/6.2));}
double CaS::tau_m(double V, double Ca) {return pow_Q_tau_m_delta_temp*(2.8 + 14.0/(exp((V+27.0)/10.0) + exp((V+70.0)/-13.0)));}
double CaS::tau_h(double V, double Ca) {return pow_Q_tau_h_delta_temp*(120.0 + 300.0/(exp((V+55.0)/9.0) + exp((V+65.0)/-16.0)));}

#endif
