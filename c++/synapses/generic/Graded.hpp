// genric graded synapse
// with a user-controllable half-maximum for activation
// and timescale 
#pragma once
#include "synapse.hpp"

class Graded: public synapse {

public:

    double Delta = 5.0;
    double tau = 100;
    double Vth = -35.0;


    // specify parameters + initial conditions
    Graded(double gmax_, double s_, double Vth_, double Delta_, double tau_)
    {
        gmax = gmax_;
        E = -80.0;
        Vth = Vth_;
        tau = tau_;
        Delta = Delta_;

        // dynamic variables
        s = s_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gmax)) { gmax = 0; }
        if (isnan (Vth)) { Vth = -35.0; }
        if (isnan (tau)) { tau = 100; }
        is_electrical = false;

        fullStateSize = 2;
        name = "Graded";
    }

    void integrate(void);
    void integrateMS(int, double, double);
    void checkSolvers(int);

    double s_inf(double);
    double tau_s(double);
    double sdot(double, double);

    int getFullState(double*, int);
};



double Graded::s_inf(double V_pre) {
    return 1.0/(1.0+exp((Vth - V_pre)/Delta));
}

double Graded::tau_s(double V_pre) {
    return tau*(1 - s_inf(V_pre));
}

double Graded::sdot(double V_pre, double s_) {
    double sinf = s_inf(V_pre);
    return (sinf - s_)/tau_s(V_pre);
}

void Graded::integrate(void) {
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;
    double sinf = s_inf(V_pre);

    // integrate using exponential Euler
    s = sinf + (s - sinf)*exp(-dt/tau_s(V_pre));

    g = gmax*s;


}

void Graded::integrateMS(int k, double V, double Ca) {

    double V_pre;



    if (k == 0) {
        V_pre = pre_syn->V_prev;
        k_s[0] = dt*(sdot(V_pre, s));

    } else if (k == 1) {

        V_pre = pre_syn->V_prev + pre_syn->k_V[0]/2;
        k_s[1] = dt*(sdot(V_pre, s + k_s[0]/2));


    } else if (k == 2) {

        V_pre = pre_syn->V_prev + pre_syn->k_V[1]/2;
        k_s[2] = dt*(sdot(V_pre, s + k_s[1]/2));

    } else if (k == 3) {
        V_pre = pre_syn->V_prev + pre_syn->k_V[2];
        k_s[3] = dt*(sdot(V_pre, s + k_s[2]));


    } else {
        // last step

        s = s + (k_s[0] + 2*k_s[1] + 2*k_s[2] + k_s[3])/6;

        if (s < 0) {s = 0;}
        if (s > 1) {s = 1;}
    }

}

void Graded::checkSolvers(int k){
    if (k == 0) {
        return;
    } else if (k == 4) {
        return;
    }
    mexErrMsgTxt("[Graded] Unsupported solver order\n");
}

int Graded::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = getCurrent(post_syn->V);
    idx++;
    return idx;
}


