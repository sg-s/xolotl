// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// calcium current
// Traub et al. 1991
// https://www.researchgate.net/publication/21491281_A_Model_of_CA3_Hippocampal_Pyramidal_Neuron_Incorporating_Voltage-Clamp_Data_on_Intrinsic_Conductances

#ifndef CAL
#define CAL
#include "conductance.hpp"

//inherit conductance class spec
class Cal: public conductance {

public:

    // specify parameters + initial conditions
    Cal(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }
    }

    void integrate(double V, double Ca, double delta_temp);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};


string Cal::getClass(){
    return "Cal";
}

void Cal::integrate(double V, double Ca, double delta_temp)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*h;
}

double Cal::m_inf(double V, double Ca) {return (1.6/(1.0+exp(-0.072*(V-65.0))))/((1.6/(1.0+exp(-0.072*(V-65.0))))+(0.02*(V-51.1)/(exp((V-51.1)/5.0)-1.0))); }
double Cal::tau_m(double V, double Ca) {return 1.0/((1.6/(1.0+exp(-0.072*(V-65.0))))+(0.02*(V-51.1)/(exp((V-51.1)/5.0)-1.0)))}

double Cal::h_inf(double V, double Ca) {
    if (V <= 0.0) {
        return 1.0;
    }
    if (V > 0.0) {
        return (exp(-V/20.0)/200.0)/((exp(-V/20.0)/200.0)+(0.005*exp(-V/20.0)/200.0));
    }
}

double Cal::tau_h(double V, double Ca) {
        if (V <= 0.0) {
            return 1.0 / (0.005 + 0.0);
        }
        if (V > 0.0) {
            return 1.0/((exp(-V/20.0)/200.0)+(0.005*exp(-V/20.0)/200.0));
        }
    }
}


#endif
