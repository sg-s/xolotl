// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: temperature-sensitive Calcium buffering and influx mechanism
// component source [Buchholtz et al. 1992](https://doi.org/10.1152/jn.1992.67.2.332)
// and in "Methods in neuronal modelling"
// and others.

#ifndef CALCIUMMECH
#define CALCIUMMECH
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class CalciumMech: public mechanism {

protected:
    double dt_by_tau_Ca = 0;
    double delta_temp = 0;
    double compensated_tau_Ca = 1;
public:



    double Q_tau = 1;

    // parameters for calciumMech2
    double tau_Ca = 200;  // milliseconds

    // parameter to convert from i_Ca to uM
    double phi = 906.4649968;

    double Ca_in = .05;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumMech(double tau_Ca_, double phi_, double Ca_in_, double Q_tau_)
    {
        phi = phi_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;
        Q_tau = Q_tau_;

        name = "CalciumMech";

        controlling_class = "unset";

    }


    void integrate(void);
    void checkSolvers(int);
    void connectCompartment(compartment*);
    void init(void);


};



// connection methods
void CalciumMech::connectCompartment(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}



void CalciumMech::init() {
    if (isnan(comp->vol)) {mexErrMsgTxt("[CalciumMech] this mechanism requires that the volume of the compartment it is in be defined. \n");}

    delta_temp = (temperature - temperature_ref)/10;
    compensated_tau_Ca = tau_Ca/((pow(Q_tau, delta_temp)));

    dt_by_tau_Ca = exp(-dt/(compensated_tau_Ca));

}

void CalciumMech::integrate(void) {

    double Ca = comp->Ca_prev;

    // this convoluted method is so that we can rewrite the
    // calcium equation in an exponential form and
    // then use the Exponential Euler method to solve it
    // otherwise this becomes very tricky and easily
    // diverges

    double Ca_inf = Ca_in - (tau_Ca*phi*(comp->i_Ca_prev)*(comp->A))/(192971*(comp->vol)); // microM
    comp->Ca = Ca_inf + (Ca - Ca_inf)*dt_by_tau_Ca;


    // Euler formulation
    // double Cadot = -(phi*(comp->i_Ca_prev)*(comp->A))/((192971*(comp->vol)))  - (Ca - Ca_in)/compensated_tau_Ca;
    // comp->Ca = Ca + Cadot*dt;
}




void CalciumMech::checkSolvers(int k) {
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech] unsupported solver order\n");
    }
}



#endif
