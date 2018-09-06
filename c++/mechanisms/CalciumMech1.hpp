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
    void integrateMS(int, double, double);

    double Cadot(double);

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

double CalciumMech1::Cadot(double Ca_)
{
    return -(f*(comp->i_Ca)*(comp->A) - Ca_ + Ca_in)/tau_Ca;
}

// Runge-Kutta 4 integrator 
void CalciumMech1::integrateMS(int k, double V, double Ca_)
{
    if (k == 4){return;}

    comp->k_Ca[k] = dt*(Cadot(Ca_));
    // mexPrintf("k_Ca[k] = %f\n", comp->k_Ca[k]);
}



#endif
