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

public:

    //specify both gbar and erev and initial conditions
    HCurrent(double g_, double E_, double m_, double Q_g_, double Q_tau_m_)
    {
        gbar = g_;
        E = E_;
        m = m_;


        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (Q_g)) { Q_g = 1; }
        if (isnan (Q_tau_m)) { Q_tau_m = 1; }
        if (isnan (E)) { E = -20; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};

string ACurrent::getClass(){
    return "HCurrent";
}

void HCurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V));
    g = pow(Q_g, delta_temp)*gbar*m;
}


double HCurrent::m_inf(double V) {return 1.0/(1.0+exp((V+75.0)/5.5));}
double HCurrent::tau_m(double V) {return (2/( exp((V+169.7)/(-11.6)) + exp((V- 26.7)/(14.3)) ));}


#endif
