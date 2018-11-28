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


    // parameters for calciumMech1
    double tau_Ca = 200;  // milliseconds

    // parameter to convert from i_Ca to uM
    double f = 14.96;

    double Ca_in = .05;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumMech(double tau_Ca_, double f_, double Ca_in_)
    {
        f = f_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;

        controlling_class = "unset";

    }

    void checkSolvers(int);

    void integrate(void);
    void integrateMS(int, double, double);

    double Cadot(double);

    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);


    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

};


double CalciumMech::getState(int idx){return std::numeric_limits<double>::quiet_NaN();}


int CalciumMech::getFullStateSize(){return 0; }


int CalciumMech::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}

// connection methods
void CalciumMech::connect(compartment* comp_)
{
    comp = comp_;
    comp->addMechanism(this);
}

void CalciumMech::connect(conductance* cond_)
{
    mexErrMsgTxt("[CalciumMech] This mechanism cannot connect to a conductance object");
}

void CalciumMech::connect(synapse* syn_)
{
    mexErrMsgTxt("[CalciumMech] This mechanism cannot connect to a synapse object");
}



void CalciumMech::integrate(void)
{
    double Ca = comp->Ca_prev;
    double Ca_inf = Ca_in - f*(comp->A)*(comp->i_Ca_prev);
    Ca = Ca_inf + (Ca - Ca_inf)*exp(-dt/tau_Ca);
    comp->Ca = Ca;

}

double CalciumMech::Cadot(double Ca_)
{
    return (-f*(comp->i_Ca)*(comp->A) - Ca_ + Ca_in)/tau_Ca;
}

// Runge-Kutta 4 integrator
void CalciumMech::integrateMS(int k, double V, double Ca_)
{
    if (k == 4){return;}

    comp->k_Ca[k] = dt*(Cadot(Ca_));
    // mexPrintf("k_Ca[k] = %f\n", comp->k_Ca[k]);
}

void CalciumMech::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else if (k == 4){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech] unsupported solver order\n");
    }
}



#endif
