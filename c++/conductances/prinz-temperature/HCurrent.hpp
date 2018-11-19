// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// H current. again, for mysterious reasons, the compiler
// won't let me call this class "H"
// http://jn.physiology.org/content/jn/90/6/3998.full.pdf
#ifndef HCURRENT
#define HCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {

private: 
    double delta_temp = 0;
    double pow_Q_tau_m_delta_temp = 0;
    double pow_Q_g = 0;


public:

    double Q_g;
    double Q_tau_m;
    double Q_tau_h;

    //specify both gbar and erev and initial conditions
    HCurrent(double g_, double E_, double m_, double Q_g_, double Q_tau_m_)
    {
        gbar = g_;
        E = E_;
        m = m_;


        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (Q_g)) { Q_g = 1; }
        if (isnan (Q_tau_m)) { Q_tau_m = 2; }
        if (isnan (E)) { E = -20; }
    }

    void integrate(double, double);
    void connect(compartment*);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string HCurrent::getClass(){
    return "HCurrent";
}

void HCurrent::connect(compartment *pcomp_) {
    // call super class method
    conductance::connect(pcomp_);

    // also set up some useful things

    delta_temp = (temperature - temperature_ref)/10;
    pow_Q_tau_m_delta_temp = (dt*pow(Q_tau_m, delta_temp));
    pow_Q_g = pow(Q_g, delta_temp);
}

void HCurrent::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-pow_Q_tau_m_delta_temp/tau_m(V,Ca));
    g = pow_Q_g*gbar*m;
}


double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+75.0)/5.5));}
double HCurrent::tau_m(double V, double Ca) {return (2/( exp((V+169.7)/(-11.6)) + exp((V- 26.7)/(14.3)) ));}


#endif
