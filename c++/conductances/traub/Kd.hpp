// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// fast sodium current
// Traub et al. 1991
// https://www.researchgate.net/publication/21491281_A_Model_of_CA3_Hippocampal_Pyramidal_Neuron_Incorporating_Voltage-Clamp_Data_on_Intrinsic_Conductances

#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

public:

    // specify parameters + initial conditions
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

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};


string Kd::getClass(){
    return "Kd";
}

void Kd::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m;
}

double Kd::m_inf(double V) {return (0.016*(35.1-V)/(exp((35.1-V)/5.0)-1.0))/((0.016*(35.1-V)/(exp((35.1-V)/5.0)-1.0))+(0.25*exp((20.0-V)/40.0))); }
double Kd::tau_m(double V) {return 1.0/((0.016*(35.1-V)/(exp((35.1-V)/5.0)-1.0))+(0.25*exp((20.0-V)/40.0)));}


#endif
