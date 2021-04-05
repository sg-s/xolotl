// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: temperature-sensitive Calcium buffering and influx mechanism
// component source [Prinz et al. 2003](https://journals.physiology.org/doi/full/10.1152/jn.00641.2003)


#ifndef CALCIUMMECH
#define CALCIUMMECH
#include <limits>


//inherit controller class spec
class CalciumMech: public mechanism {

protected:
    double dt_by_tau_Ca = 0;
    double delta_temp = 0;
public:


    // parameters for calciumMech
    double tau_Ca = 200;  // milliseconds
    double Q_tau = 1;

    // parameter to convert from i_Ca to uM
    double f = 14.96;

    double Ca_in = .05;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumMech(double tau_Ca_, double f_, double Ca_in_, double Q_tau_)
    {
        f = f_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;
        Q_tau = Q_tau_;

        controlling_class = "unset";
        name = "CalciumMech";

    }

    void integrate(void);
    void init(void);

    

};



void CalciumMech::init() {
    delta_temp = (temperature - temperature_ref)/10;
    dt_by_tau_Ca = exp(-dt/(tau_Ca)*(pow(Q_tau, delta_temp)));
}


void CalciumMech::integrate(void) {
    double Ca = comp->Ca_prev;
    double Ca_inf = Ca_in - f*(comp->A)*(comp->i_Ca_prev);
    Ca = Ca_inf + (Ca - Ca_inf)*dt_by_tau_Ca;
    comp->Ca = Ca;

}



#endif
