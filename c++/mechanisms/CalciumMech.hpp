// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// CalciumMech mechanism 
// as in Prinz et al 2003 (and others)

#ifndef CALCIUMMECH
#define CALCIUMMECH
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class CalciumMech: public mechanism {

protected:
public:


    // parameters for calciumMech 
    double tau_Ca; 

    // parameter to convert from i_Ca to uM 
    double f; 

    double Ca_in;

    // specify parameters + initial conditions for 
    // mechanism that controls a conductance 
    CalciumMech(double tau_Ca_, double f_, double Ca_in_)
    {
        f = f_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;

        if (isnan (tau_Ca)) { tau_Ca = 200; }
        if (isnan (Ca_in)) { Ca_in = .05; }
        if (isnan (f)) { f = 14.96; }

        controlling_class = "unset";

    }

    
    void integrate(double dt);
    void connect(compartment * comp_);
    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

};


double CalciumMech::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();

}


int CalciumMech::getFullStateSize()
{
    return 0; 
}


int CalciumMech::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}


void CalciumMech::connect(compartment* comp_)
{
    comp = comp_;
}

void CalciumMech::integrate(double dt)
{


    double Ca = comp->Ca_prev;
    double Ca_inf = Ca_in - f*(comp->A)*(comp->i_Ca_prev);
    Ca = Ca_inf + (Ca - Ca_inf)*exp(-dt/tau_Ca);
    comp->Ca = Ca;

}



#endif
