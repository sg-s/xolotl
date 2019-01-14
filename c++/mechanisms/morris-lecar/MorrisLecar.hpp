// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// morris-lecar model implemented as a mechanism
//
//
//
#ifndef MORRISLECAR
#define MORRISLECAR
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class MorrisLecar: public mechanism {

protected:

public:

    // default parameter values
    double gbar_Leak = 20;
    double gbar_Ca = 44;
    double gbar_Kd = 80;
    double E_Leak = -50;
    double E_Ca = 120;
    double E_Kd = -80;
    double phi = .04;
    double v1 = -1.2;
    double v2 = 18;
    double v3 = 2;
    double v4 = 30;

    // default state variable values
    double n = 0;

    // containers for multi-step integration
    double k_n[4] = {0,0,0,0};
    double k_v[4] = {0,0,0,0};

    // constructor
    MorrisLecar(double gbar_Leak_, double gbar_Kd_, double gbar_Ca_, double E_Kd_, double E_Ca_, double E_Leak_, double phi_, double v1_, double v2_, double v3_, double v4_, double n_)
    {
        //  parameters
        gbar_Leak = gbar_Leak_;
        gbar_Kd = gbar_Kd_;
        gbar_Ca = gbar_Ca_;
        E_Kd    = E_Kd_;
        E_Ca    = E_Ca_;
        E_Leak  = E_Leak_;
        phi     = phi_;
        v1      = v1_;
        v2      = v2_;
        v3      = v3_;
        v4      = v4_;

        // state variables
        n       = n_;

        if (isnan(n)) {n = 0;}
    }

    void checkSolvers(int);
    void integrate(void);
    void integrateMS(int, double, double);

    double Vdot(double, double);
    double Ndot(double, double);

    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    int getFullState(double* cont_state, int idx);
    int getFullStateSize(void);

    double getState(int);

};

// bookkeeping methods
double MorrisLecar::getState(int idx) {
    return n;
}

int MorrisLecar::getFullStateSize() {
    return 1;
}

int MorrisLecar::getFullState(double *cont_state, int idx) {
    // return n, the state variable
    cont_state[idx] = n;
    idx++;
    return idx;
}

// connection methods
void MorrisLecar::connect(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}

void MorrisLecar::connect(conductance* cond_) {
    mexErrMsgTxt("[MorrisLecar] This mechanism cannot connect to a conductance object");
}

void MorrisLecar::connect(synapse* syn_) {
    mexErrMsgTxt("[MorrisLecar] This mechanism cannot connect to a synapse object\n");
}

double MorrisLecar::Vdot(double v, double n) {
    double m_inf = 0.5 * (1 + tanh((v - v1)/v2));
    return (-gbar_Leak * (v - E_Leak) - gbar_Ca * m_inf *  (v - E_Ca) - gbar_Kd * n * (v - E_Kd))/Cm;
}

double MorrisLecar::Ndot(double v, double n) {
    double n_inf = 0.5 * (1 + tanh((v - v1)/v2));
    double n_tau = 1.0 / (phi * cosh((v - v3)/(2*v4)));
    return (n_inf - n) / n_tau;
}

// integration methods
void MorrisLecar::integrate(void) {
    // euler's method
    double v    = comp->V;
    n           = n + Ndot(v, n) * dt;
    v           = v + Vdot(v, n) * dt;
    comp->V_prev     = v;
}

void MorrisLecar::integrateMS(int k, double v, double Ca_) {
    switch (k)
    {
        case 0:
            k_n[0] = dt * Ndot(v, n);
            k_v[0] = dt * Vdot(v, n);
        case 1:
            k_n[1] = dt * Ndot(v, n + k_n[0]/2);
            k_v[1] = dt * Vdot(v + k_v[0]/2, n);
        case 2:
            k_n[2] = dt * Ndot(v, n + k_n[1]/2);
            k_v[2] = dt * Vdot(v + k_v[1]/2, n);
        case 3:
            k_n[3] = dt * Ndot(v, n + k_n[2]);
            k_v[3] = dt * Vdot(v + k_v[2], n);
        case 4:
            // last step
            n = n + (k_n[0] + 2*k_n[1] + 2*k_n[2] + k)
            m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
    }
}

void MorrisLecar::checkSolvers(int k) {
    if (k == 0){
        return;
    } else if (k == 4){
        return;
    } else {
        mexErrMsgTxt("[MorrisLecar] unsupported solver order\n");
    }
}




#endif
