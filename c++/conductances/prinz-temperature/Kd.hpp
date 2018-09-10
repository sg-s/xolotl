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

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string Kd::getClass(){return "Kd";}

void Kd::integrate(double V, double Ca)
{
    double delta_temp = (temperature - temperature_ref)/10;
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V,Ca));
    g = pow(Q_g, delta_temp)*gbar*m*m*m*m;

}



double Kd::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+12.3)/-11.8));}
double Kd::tau_m(double V, double Ca) {return 14.4 - 12.8/(1.0+exp((V+28.3)/-19.2));}


#endif
