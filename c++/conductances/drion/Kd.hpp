// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Drion 2011
// http://www.ele.uri.edu/faculty/vetter/BME307/Projects/neuron-dopamine-project.pdf
#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

public:

    //specify both gbar and erev and initial conditions
    Kd(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);


};

string Kd::getClass(){
    return "Kd";
}

void Kd::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m*m*m*m;
}

double Kd::m_inf(double V) {return ((0.25)*(V+55.0))/((100)*(1-exp(-0.1*((V)+55.0)))) / ( ((0.25)*(V+55.0))/((100)*(1-exp(-0.1*((V)+55.0)))) + ((0.03125)*exp(((V)+65.0)/-80.0)) );}
double Kd::tau_m(double V) {return 1.0 / ( ((0.25)*(V+55.0))/((100)*(1-exp(-0.1*((V)+55.0)))) + ((0.03125)*exp(((V)+65.0)/-80.0)) );}

#endif
