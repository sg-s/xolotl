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
    NaV(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);

};


string NaV::getClass(){
    return "NaV";
}

void NaV::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double NaV::m_inf(double V) {return (0.32*(13.1-V)/(exp((13.1-V)/4.0)-1.0))/((0.32*(13.1-V)/(exp((13.1-V)/4.0)-1.0))+(0.28*(V-40.1)/(exp((V-40.1)/5)-1.0))); }
double NaV::h_inf(double V) {return (0.128*exp((17.0-V)/18.0))/((0.128*exp((17.0-V)/18.0))+(4.0/(1.0+exp((40.0-V)/5.0)))); }
double NaV::tau_m(double V) {return (1.0)/((0.32*(13.1-V)/(exp((13.1-V)/4.0)-1.0))+(0.28*(V-40.1)/(exp((V-40.1)/5)-1.0)));}
double NaV::tau_h(double V) {return (1.0)/((0.128*exp((17.0-V)/18.0))+(4.0/(1.0+exp((40.0-V)/5.0))));}

#endif
