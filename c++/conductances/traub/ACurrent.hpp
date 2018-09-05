//  _ _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// A-type conductance
// Traub et al. 1991
// https://www.researchgate.net/publication/21491281_A_Model_of_CA3_Hippocampal_Pyramidal_Neuron_Incorporating_Voltage-Clamp_Data_on_Intrinsic_Conductances

#ifndef ACURRENT
#define ACURRENT
#include "conductance.hpp"

//inherit conductance class spec
class ACurrent: public conductance {

public:

    // specify parameters + initial conditions
    ACurrent(double g_, double E_, double m_, double h_)
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

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);

};


string ACurrent::getClass(){
    return "ACurrent";
}

void ACurrent::integrate(double V, double Ca, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*h;
}

double ACurrent::m_inf(double V) {return (0.02*(13.1-V)/(exp((13.1-V)/10.0)-1.0))/((0.02*(13.1-V)/(exp((13.1-V)/10.0)-1.0))+(0.0175*(V-40.1)/(exp((V-40.1)/10.0)-1.0))); }
double ACurrent::h_inf(double V) {return (0.128*exp((17.0-V)/18.0))/((0.128*exp((17.0-V)/18.0))+(4.0/(1.0+exp((40.0-V)/5.0)))); }
double ACurrent::tau_m(double V) {return (0.0016*exp((-13.0-V)/18.0))/((0.0016*exp((-13.0-V)/18.0))+(0.05/(1.0+exp((10.1-V)/5.0))));}
double ACurrent::tau_h(double V) {return 1.0/((0.0016*exp((-13.0-V)/18.0))+(0.05/(1.0+exp((10.1-V)/5.0))));}

#endif
