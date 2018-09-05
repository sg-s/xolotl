// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// adapted from Adams et al. 1982 from bullfrog sympathetic neurons
// calcium-gated
// as described in Traub et al. 1991
// https://www.researchgate.net/publication/21491281_A_Model_of_CA3_Hippocampal_Pyramidal_Neuron_Incorporating_Voltage-Clamp_Data_on_Intrinsic_Conductances

#ifndef KC
#define KC
#include "conductance.hpp"

//inherit conductance class spec
class Kc: public conductance {

public:

    // specify parameters + initial conditions
    Kc(double g_, double E_, double m_)
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


string Kc::getClass(){
    return "Kc";
}

void Kc::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar * m * min(1.0, Ca / 250.0);
}

double Kc::m_inf(double V, double Ca) {
    if (V <= -50.0) {
        double alpha = exp(((V-10.0)/11.0)-((V-6.5)/27.0))/18.975;
        double beta = 2*exp(-(V-6.5)/27.0) - alpha;
        return alpha / (alpha + beta);
    }
    return 1.0;
}

double Kc::tau_m(double V, double Ca) {
    if (V <= -50.0) {
        double alpha = exp(((V-10.0)/11.0)-((V-6.5)/27.0))/18.975;
        double beta = 2*exp(-(V-6.5)/27.0) - alpha;
        return 1.0 / (alpha + beta);
    }
    return alpha + beta;
}

#endif
