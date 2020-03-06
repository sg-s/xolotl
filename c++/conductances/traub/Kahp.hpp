// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// after-hyperpolarization potassium conductance
// Traub et al. 1991
// https://www.researchgate.net/publication/21491281_A_Model_of_CA3_Hippocampal_Pyramidal_Neuron_Incorporating_Voltage-Clamp_Data_on_Intrinsic_Conductances

#ifndef KAHP
#define KAHP
#include "conductance.hpp"

//inherit conductance class spec
class Kahp: public conductance {

public:

    // specify parameters + initial conditions
    Kahp(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        p = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }
    }
    
    double m_inf(double V, double Ca);
    double tau_m(double, double);
    string getClass(void);

};


string Kahp::getClass(){
    return "Kahp";
}

double Kahp::m_inf(double V, double Ca) {
    double var = 0.2 / 10000 * Ca;
    if (var < 0.01) { return var / (var + 0.001); }
    return 0.01 / (0.01 + 0.001);
}
double Kahp::tau_m(double V, double Ca) {
    double var = 0.2 / 10000 * Ca;
    if (var < 0.01) { return 1.0 / (var + 0.001); }
    return 1.0 / (0.01 + 0.001);
}

#endif
