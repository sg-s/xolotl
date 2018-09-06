// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// CalciumMech1 mechanism 
// as in Prinz et al 2003 (and others)

#ifndef CALCIUMMECH1
#define CALCIUMMECH1
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class CalciumMech1: public mechanism {

protected:
public:


    // parameters for calciumMech1 
    double tau_Ca = 200;  // milliseconds

    // parameter to convert from i_Ca to uM 
    double f = 14.96; 

    double Ca_in = .05;

    // specify parameters + initial conditions for 
    // mechanism that controls a conductance 
    CalciumMech1(double tau_Ca_, double f_, double Ca_in_)
    {
        f = f_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;

        controlling_class = "unset";

    }

    
    void integrate(void);
    void connect(compartment * comp_);
    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

};


double CalciumMech1::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();

}


int CalciumMech1::getFullStateSize()
{
    return 0; 
}


int CalciumMech1::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}


void CalciumMech1::connect(compartment* comp_)
{
    comp = comp_;
}

void CalciumMech1::integrate(void)
{
    double Ca = comp->Ca_prev;
    double Ca_inf = Ca_in - f*(comp->A)*(comp->i_Ca_prev);
    Ca = Ca_inf + (Ca - Ca_inf)*exp(-dt/tau_Ca);
    comp->Ca = Ca;

}

// Runge-Kutta 4 integrator 
void CalciumMech1::integrateMS(int k, double V, double Ca)
{
    if (k == 0) {
        k_m[0] = dt*(mdot(V, Ca, m));
        k_h[0] = dt*(hdot(V, Ca, h));
        g = gbar*pow(m,p)*pow(h,q);
    } else if (k == 1) {
        k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
        k_h[1] = dt*(hdot(V, Ca, h + k_h[0]/2));
        g = gbar*pow(m + k_m[0]/2,p)*pow(h + k_h[0]/2,q);

    } else if (k == 2) {
        k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
        k_h[2] = dt*(hdot(V, Ca, h + k_h[1]/2));
        g = gbar*pow(m + k_m[1]/2,p)*pow(h + k_h[1]/2,q);

    } else if (k == 3) {
        k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
        k_h[3] = dt*(hdot(V, Ca, h + k_h[2]));
        g = gbar*pow(m + k_m[2],p)*pow(h + k_h[2],q);

    } else {
        // last step
        m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
        h = h + (k_h[0] + 2*k_h[1] + 2*k_h[2] + k_h[3])/6;
    }
}



#endif
