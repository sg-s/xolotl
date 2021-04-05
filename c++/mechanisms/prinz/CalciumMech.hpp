// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Calcium buffering and influx mechanism
// component source [Prinz et al. 2003](https://journals.physiology.org/doi/full/10.1152/jn.00641.2003)


#ifndef CALCIUMMECH
#define CALCIUMMECH
#include <limits>


//inherit controller class spec
class CalciumMech: public mechanism {

protected:
    double dt_by_tau_Ca = 0;
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
        name = "CalciumMech";

    }

    void checkSolvers(int);

    void integrate(void);
    void integrateMS(int, double, double);

    double Cadot(double);

    void init(void);

};



void CalciumMech::init() {
    dt_by_tau_Ca = exp(-dt/tau_Ca);
}



void CalciumMech::integrate(void) {
    double Ca = comp->Ca_prev;
    double Ca_inf = Ca_in - f*(comp->A)*(comp->i_Ca_prev);
    Ca = Ca_inf + (Ca - Ca_inf)*dt_by_tau_Ca;
    comp->Ca = Ca;

}

double CalciumMech::Cadot(double Ca_) {
    return (-f*(comp->i_Ca)*(comp->A) - Ca_ + Ca_in)/tau_Ca;
}

// Runge-Kutta 4 integrator
void CalciumMech::integrateMS(int k, double V, double Ca_) {
    if (k == 4){return;}

    comp->k_Ca[k] = dt*(Cadot(Ca_));
    // mexPrintf("k_Ca[k] = %f\n", comp->k_Ca[k]);
}

void CalciumMech::checkSolvers(int k) {
    if (k == 0){
        return;
    } else if (k == 4){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech] unsupported solver order\n");
    }
}



#endif
