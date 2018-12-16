// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// delayed rectifier
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

        p = 4;

        // allow this channel to be approximated
        approx_m = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};


string Kd::getClass(){
    return "Kd";
}

double Kd::m_inf(double V, double Ca) {return (0.016*(35.1-V)/(exp((35.1-V)/5.0)-1.0))/((0.016*(35.1-V)/(exp((35.1-V)/5.0)-1.0))+(0.25*exp((20.0-V)/40.0))); }
double Kd::tau_m(double V, double Ca) {return 1.0/((0.016*(35.1-V)/(exp((35.1-V)/5.0)-1.0))+(0.25*exp((20.0-V)/40.0)));}


#endif
