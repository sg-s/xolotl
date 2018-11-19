// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// inward rectifying potassium conductance
// http://jn.physiology.org/content/jn/90/6/3998.full.pdf
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
    Kd(double g_, double E_, double m_, double Q_g_, double Q_tau_m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (Q_g)) { Q_g = 1; }
        if (isnan (Q_tau_m)) { Q_tau_m = 2; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double, double);
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
    pow_Q_tau_m_delta_temp = (dt*pow(Q_tau_m, delta_temp));
    pow_Q_g = pow(Q_g, delta_temp);
}

void Kd::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-pow_Q_tau_m_delta_temp/tau_m(V,Ca));
    g = pow_Q_g*gbar*m*m*m*m;

}



double Kd::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+12.3)/-11.8));}
double Kd::tau_m(double V, double Ca) {return 14.4 - 12.8/(1.0+exp((V+28.3)/-19.2));}


#endif
