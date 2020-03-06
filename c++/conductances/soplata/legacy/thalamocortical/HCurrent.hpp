// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// hyperpolarization-activated inward rectifying mixed cation current
//
// "Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N.
// J. (2010). Thalamocortical model for a propofol-induced alpha-rhythm
// associated with loss of consciousness. Proceedings of the National Academy of
// Sciences, 107(52), 22665-22670. doi:10.1073/pnas.1017069108"

#ifndef HCURRENT
#define HCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {

public:

    // dynamical variables
    double C1;
    double P0;
    double O1;

    // containers for multi-step solvers
    double k_C1[4] = {0,0,0,0};
    double k_P0[4] = {0,0,0,0};
    double k_O1[4] = {0,0,0,0};

    //specify both gbar and erev and initial conditions
    HCurrent(double gbar_, double E_, double C1_, double P0_, double O1_)
    {
        gbar = gbar_;
        E = E_;

        C1 = C1_;
        P0 = P0_;
        O1 = O1_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (C1)) { C1 = 0; }
        if (isnan (P0)) { P0 = 0; }
        if (isnan (O1)) { O1 = 0; }
        if (isnan (E)) { E = -43; }

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

    // equations of state
    double C1dot(double, double, double, double, double);
    double P0dot(double, double, double, double, double);
    double O1dot(double, double, double, double, double);

    // auxiliary functions
    double aH(double, double);
    double bH(double, double);

    // use non-default integration methods
    void integrate(double, double, double, double, double);
    void integrateMS(int, double, double, double, double, double);

};

string HCurrent::getClass(){
    return "HCurrent";
}

double HCurrent::aH(double V, double Ca) {
    return (1.0 - m_inf(V, Ca)) / tau_m(V, Ca);
}

double HCurrent::bH(double V, double Ca) {
    return m_inf(V, Ca) / tau_m(V, Ca);
}

double HCurrent::C1dot(double V, double Ca, double C1, double O1, double P1) {
    return aH(V, Ca) * O1 - bH(V, Ca) * C1;
}

double HCurrent::P0dot(double V, double Ca, double C1, double O1, double P1) {
    return 0.0004*(1.0-P0) -6.4e-15*(Ca*Ca*Ca*Ca)*P0;
}

double HCurrent::O1dot(double V, double Ca, double C1, double O1, double P1) {
    return 0.001*(1.0-C1-O1) - 0.1*P0*O1;
}

// implement forward Euler
void HCurrent::integrate(double V, double Ca, double C1, double O1, double P1) {
    C1 = C1 + dt*C1dot(V, Ca, C1, O1, P1);
    P0 = P0 + dt*P0dot(V, Ca, C1, O1, P1);
    O1 = O1 + dt*O1dot(V, Ca, C1, O1, P1);

    g = gbar * (O1 + 2.0 * (1.0 - C1 - O1));

}

void HCurrent::integrateMS(int k, double V, double Ca, double C1, double O1, double P1) {

    switch (k)
    {
        case 0:
            k_C1[0] = dt*C1dot(V, Ca, C1, O1, P0);
            k_O1[0] = dt*O1dot(V, Ca, C1, O1, P0);
            k_P0[0] = dt*P0dot(V, Ca, C1, O1, P0);
            g = gbar * (O1 + 2.0 * (1.0 - C1 - O1));
            break;
        case 1:
            k_C1[1] = dt*C1dot(V, Ca, C1 + k_C1[0]/2, O1 + k_O1[0]/2, P0 + k_P0[0]/2);
            k_O1[1] = dt*O1dot(V, Ca, C1 + k_C1[0]/2, O1 + k_O1[0]/2, P0 + k_P0[0]/2);
            k_P0[1] = dt*P0dot(V, Ca, C1 + k_C1[0]/2, O1 + k_O1[0]/2, P0 + k_P0[0]/2);
            g = gbar * ((O1+k_O1[0]/2) + 2.0 * (1.0 - (C1+k_C1[0]/2) - (O1+k_O1[0]/2)));
            break;
        case 2:
            k_C1[2] = dt*C1dot(V, Ca, C1 + k_C1[1]/2, O1 + k_O1[1]/2, P0 + k_P0[1]/2);
            k_O1[2] = dt*O1dot(V, Ca, C1 + k_C1[1]/2, O1 + k_O1[1]/2, P0 + k_P0[1]/2);
            k_P0[2] = dt*P0dot(V, Ca, C1 + k_C1[1]/2, O1 + k_O1[1]/2, P0 + k_P0[1]/2);
            g = gbar * ((O1+k_O1[1]/2) + 2.0 * (1.0 - (C1+k_C1[1]/2) - (O1+k_O1[1]/2)));
            break;
        case 3:
            k_C1[3] =  dt*C1dot(V, Ca, C1 + k_C1[2]/2, O1 + k_O1[2]/2, P0 + k_P0[2]/2);
            k_O1[3] =  dt*O1dot(V, Ca, C1 + k_C1[2]/2, O1 + k_O1[2]/2, P0 + k_P0[2]/2);
            k_P0[3] =  dt*P0dot(V, Ca, C1 + k_C1[2]/2, O1 + k_O1[2]/2, P0 + k_P0[2]/2);
            g = gbar * ((O1+k_O1[2]/2) + 2.0 * (1.0 - (C1+k_C1[2]/2) - (O1+k_O1[2]/2)));
            break;
        case 4:
            C1 = C1 + (k_C1[0] + 2*k_C1[1] + 2*k_C1[2] + k_C1[3])/6;
            O1 = O1 + (k_O1[0] + 2*k_O1[1] + 2*k_O1[2] + k_O1[3])/6;
            O1 = O1 + (k_O1[0] + 2*k_O1[1] + 2*k_O1[2] + k_O1[3])/6;
            break;
    }

}

double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+75.0)/5.5));}
double HCurrent::tau_m(double V, double Ca) {return 20.0 + 1000.0/(exp((V+71.5)/14.2)+exp((-(V+89.0))/11.6));}


#endif
