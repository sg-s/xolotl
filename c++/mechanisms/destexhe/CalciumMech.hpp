// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Calcium mechanism
//
// component info: A modified version of the Buchholtz Calcium mechanism
// component source [Destexhe et al. 1993](https://www.sciencedirect.com/science/article/pii/S0006349593811901)
// 
// The salient difference is that calcium can only leak out, it can't flow out through calcium currents
//
// Converting between parameters in the original paper and xolotl:
//      'vol' = 'A' * 'depth'
//      'phi' = 1
//
// 
// Ionic mechanisms for intrinsic slow oscillations in thalamic relay neurons.
// Biophysical journal, 65(4):1538–52
// 

#pragma once
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class CalciumMech: public mechanism {

protected:
    double dt_by_tau_Ca = 0;
public:


    // parameters for calciumMech2
    double tau_Ca = 1000;  // milliseconds

    // parameter to convert from i_Ca to uM
    double phi = 1;

    double Ca_in = 0.24; // microM (Destexhe et al. 1994, results in E_Ca = 120 mV)

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumMech(double tau_Ca_, double phi_, double Ca_in_)
    {
        phi = phi_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;

        controlling_class = "unset";
        name = "CalciumMech";

    }


    void integrate(void);
    void integrateMS(int, double, double);

    void checkSolvers(int);

    void connectCompartment(compartment*);

    

    double Cadot(double);
    double Cainf(void);

    void init(void);

};



// connection methods
void CalciumMech::connectCompartment(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}


void init() {
    if (isnan(comp_->vol)) {mexErrMsgTxt("[CalciumMech] this mechanism requires that the volume of the compartment it is in be defined. \n");}
    dt_by_tau_Ca = exp(-dt/tau_Ca);
}



void CalciumMech::integrate(void) {

    double Ca = comp->Ca_prev;

    // this convoluted method is so that we can rewrite the
    // calcium equation in an exponential form and
    // then use the Exponential Euler method to solve it
    // otherwise this becomes very tricky and easily
    // diverges

    double Ca_inf = Ca_in + (tau_Ca * Cainf()); // microM
    comp->Ca = Ca_inf + (Ca - Ca_inf)*dt_by_tau_Ca;
}

double CalciumMech::Cadot(double Ca_) { return Cainf() - (Ca_ - Ca_in)/tau_Ca; }

double CalciumMech::Cainf(void) {
    if (comp->i_Ca_prev < 0) {
        return 0.0;
    }
    else {
        return -(phi*(comp->i_Ca_prev)*(comp->A)/(192971*(comp->vol)));
    }
}

// Runge-Kutta 4 integrator
void CalciumMech::integrateMS(int k, double V, double Ca_) {
    if (k == 4){return;}
    comp->k_Ca[k] = dt*(Cadot(Ca_));
}

void CalciumMech::checkSolvers(int k) {
    if (k == 0){
        return;
    } else if (k == 4){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech1] unsupported solver order\n");
    }
}



