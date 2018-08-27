// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// CalciumMech mechanism
// as in Zhang & Golowasch, 2011
// https://link.springer.com/article/10.1007/s10827-011-0338-8

#ifndef CALCIUMMECH
#define CALCIUMMECH
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class CalciumMech: public mechanism {

protected:
public:

    // calcium-dependent activity sensor
    double Sbar = 8;

    // half-maximum for M
    double M_th = 0.9; // uM

    // conversion factor from i_Ca to uM
    double gamma = 0.00678; // uM/nA/ms

    // half-maximum pump rate
    double alpha = 0.2; // uM

    // calcium-maximum regulation
    double gbar_ca_min = 0.069; // uS
    double gbar_ca = 0.12; // uS
    double S_th = 0.13;
    double r_S = 0.02;
    double tau_A = 500000; // ms

    // calcium pump activity regulation
    double R_pump_min = 0.0026; // uM/ms
    double R_pump_M = 0.006; // uM/ms
    double M_th = 0.1331;
    double r_M = 0.00001;
    double G_th = 0.01; // uM
    double r_G = 0.00001;
    double tau_M = 2500000; // ms

    double gbar_MI = 0;

    // state and pseudo-state variables
    double S;
    double G;
    double F;
    double M;
    double R_pump;
    double G_s;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumMech(double Sbar_, double M_th_, double gamma_, double alpha_, double gbar_ca_min_, double gbar_ca_, double S_th_, double r_S_, double tau_A_, double R_pump_min_, double R_pump_M_, double M_th_, double r_M_, double G_th_, double r_G_, double tau_M_, double gbar_MI_, double S_, double G_, double F_, double M_, double R_pump_, double G_s_)
    {

      Sbar = Sbar_;
      M_th = M_t_; // uM
      gamma = gamma_; // uM/nA/ms
      alpha = alpha_; // uM
      gbar_ca_min = gbar_ca_min_; // uS
      gbar_ca = gbar_ca_; // uS
      S_th = S_th_;
      r_S = r_S_;
      tau_A = tau_A_; // ms
      R_pump_min = R_pump_min_; // uM/ms
      R_pump_M = R_pump_M_; // uM/ms
      M_th = M_th_;
      r_M = r_M_;
      G_th = G_th_; // uM
      r_G = r_G_;
      tau_M = tau_M_; // ms

      gbar_MI = gbar_MI_;

      S = S_;
      G = G_;
      F = F_;
      M = M_;
      R_pump = R_pump_;
      G_s = G_S_;

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

    double M_A = 1.0 / (1.0 + exp(M_th - Ca));
    S = Sbar*M_A*M_A*M_A*M_A;

    Ca = Ca + dt * ( - gamma * (comp->i_Ca_prev) - R_pump * (Ca * Ca) / (Ca * Ca + alpha * alpha) );

    G_s = G_s + dt / tau_A * ( gbar_ca / (1.0 + exp((S - S_th)/r_S)) - G_s );
    gbar_ca = gbar_ca_min + G_s;

    M = M + dt / tau_M * ( 1.0 / (1.0 + exp((G_th - gbar_MI) / r_G)) - M);
    F = R_pump_M / ( 1.0 + exp(- (M_th - M) / r_M) );
    R_pump = R_pump_min + F;

    comp->Ca = Ca;

}



#endif
