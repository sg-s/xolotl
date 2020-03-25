// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Calcium buffering and influx mechanism
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
public:


    // parameters for calciumMech2
    double tau_Ca = 200;  // milliseconds

    // parameter to convert from i_Ca to uM
    double phi = 906.4649968;

    double Ca_in = .05;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumMech(double tau_Ca_, double phi_, double Ca_in_)
    {
        phi = phi_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;

        controlling_class = "unset";

    }


    void integrate(void);
    void integrateMS(int, double, double);

    void checkSolvers(int);

    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

    double Cadot(double);

};


double CalciumMech::getState(int idx) {
    return std::numeric_limits<double>::quiet_NaN();
}

int CalciumMech::getFullStateSize() {
    return 0;
}

int CalciumMech::getFullState(double *cont_state, int idx) {
    // do nothing
    return idx;
}

string CalciumMech::getClass() {
    return "CalciumMech";
}

// connection methods
void CalciumMech::connect(compartment* comp_) {
    if (isnan(comp_->vol)) {mexErrMsgTxt("[CalciumMech] this mechanism requires that the volume of the compartment it is in be defined. \n");}

    comp = comp_;
    comp->addMechanism(this);

    dt_by_tau_Ca = exp(-dt/tau_Ca);
}

void CalciumMech::connect(conductance* cond_) {
    mexErrMsgTxt("[CalciumMech] This mechanism cannot connect to a conductance object");
}

void CalciumMech::connect(synapse* syn_) {
    mexErrMsgTxt("[CalciumMech] This mechanism cannot connect to a synapse object");
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
}


double CalciumMech::Cadot(double Ca_) {
    return -(phi*(comp->i_Ca)*(comp->A)/(192971*(comp->vol))) - (Ca_ - Ca_in)/tau_Ca;
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



#endif
