// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// fast sodium current
// Traub et al. 1991
// https://www.researchgate.net/publication/21491281_A_Model_of_CA3_Hippocampal_Pyramidal_Neuron_Incorporating_Voltage-Clamp_Data_on_Intrinsic_Conductances

#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions
    NaV(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 2;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 50; }
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};


string NaV::getClass(){
    return "NaV";
}

double NaV::m_inf(double V, double Ca) {return (0.32*(13.1-V)/(exp((13.1-V)/4.0)-1.0))/((0.32*(13.1-V)/(exp((13.1-V)/4.0)-1.0))+(0.28*(V-40.1)/(exp((V-40.1)/5)-1.0))); }
double NaV::h_inf(double V, double Ca) {return (0.128*exp((17.0-V)/18.0))/((0.128*exp((17.0-V)/18.0))+(4.0/(1.0+exp((40.0-V)/5.0)))); }
double NaV::tau_m(double V, double Ca) {return (1.0)/((0.32*(13.1-V)/(exp((13.1-V)/4.0)-1.0))+(0.28*(V-40.1)/(exp((V-40.1)/5)-1.0)));}
double NaV::tau_h(double V, double Ca) {return (1.0)/((0.128*exp((17.0-V)/18.0))+(4.0/(1.0+exp((40.0-V)/5.0))));}

#endif
