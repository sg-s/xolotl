// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// adapted from Adams et al. 1982 from bullfrog sympathetic neurons
// calcium-gated
// as described in Traub et al. 1991
// https://www.researchgate.net/publication/21491281_A_Model_of_CA3_Hippocampal_Pyramidal_Neuron_Incorporating_Voltage-Clamp_Data_on_Intrinsic_Conductances

#ifndef KCA
#define KCA
#include "conductance.hpp"

//inherit conductance class spec
class KCa: public conductance {

protected:

    double alpha = 0;
    double beta = 0;

public:


    // specify parameters + initial conditions
    KCa(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};


string KCa::getClass(){
    return "KCa";
}

void KCa::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    g = gbar * m * min(1.0, Ca / 250.0);
}

double KCa::m_inf(double V, double Ca) {
    if (V <= -50.0) {
        alpha = exp(((V-10.0)/11.0)-((V-6.5)/27.0))/18.975;
        beta = 2*exp(-(V-6.5)/27.0) - alpha;
        return alpha / (alpha + beta);
    }
    return 1.0;
}

double KCa::tau_m(double V, double Ca) {
    if (V <= -50.0) {
        alpha = exp(((V-10.0)/11.0)-((V-6.5)/27.0))/18.975;
        beta = 2*exp(-(V-6.5)/27.0) - alpha;
        return 1.0 / (alpha + beta);
    }
    return alpha + beta;
}

#endif
