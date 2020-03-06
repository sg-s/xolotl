// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// inward rectifying potassium conductance
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

private: 
    double delta_temp = 0;
    double pow_Q_tau_m_delta_temp = 0;
    double pow_Q_g = 0;


public:


    double Q_g;
    double Q_tau_m;
    double Q_tau_h;

    //specify both gbar and erev and initial conditions
    Kd(double gbar_, double E_, double m_, double Q_g_, double Q_tau_m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (Q_g)) { Q_g = 1; }
        if (isnan (Q_tau_m)) { Q_tau_m = 2; }
        if (isnan (E)) { E = -80; }

        p = 4;

    }

    void integrate(double, double);
    void integrateLangevin(double, double);
    void connect(compartment*);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string Kd::getClass(){return "Kd";}

void Kd::connect(compartment *pcomp_) {
    // call super class method
    conductance::connect(pcomp_);

    // also set up some useful things
    delta_temp = (temperature - temperature_ref)/10;
    pow_Q_tau_m_delta_temp = (pow(Q_tau_m, delta_temp));
    pow_Q_g = pow(Q_g, delta_temp);
}

void Kd::integrate(double V, double Ca) {
    conductance::integrate(V,Ca);
    g = pow_Q_g*g;
}

void Kd::integrateLangevin(double V, double Ca) {
    conductance::integrateLangevin(V,Ca);
    g = pow_Q_g*g;
}



double Kd::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+12.3)/-11.8));}
double Kd::tau_m(double V, double Ca) {return pow_Q_tau_m_delta_temp*(7.2 - 6.4/(1.0+exp((V+28.3)/-19.2)));}

#endif
