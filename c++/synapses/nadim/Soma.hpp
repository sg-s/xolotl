// Soma to Soma Synapse
#ifndef SOMA
#define SOMA
#include "synapse.hpp"

class Soma: public synapse {

public:

    double Delta;
    double Vth;
    double k_;

    // specify parameters + initial conditions
    Soma(double gmax_, double s_)
    {
        gmax = gmax_;
        E = -70.0;
        Delta = 1.0;
        Vth = -25.0;
        k_ = -1.0;
        s = s_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gmax)) { gmax = 0; }
        is_electrical = false;

        fullStateSize = 2;

    }

    void integrate(void);

    void connect(compartment *pcomp1_, compartment *pcomp2_);
    int getFullState(double*, int);

};

void Soma::integrate(void) {

    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;

    // find s_inf
    double s_inf = 1.0/(1.0+exp((Vth - V_pre)/Delta));

    // integrate using exponential Euler
    double tau_s = 100.0/(1.0+exp((Vth - V_pre)/k_));

    s = s_inf + (s - s_inf)*exp(-dt/tau_s);

    g = gmax*s;

}


int Soma::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;
    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}




void Soma::connect(compartment *pcomp1_, compartment *pcomp2_) {
    pre_syn = pcomp1_;
    post_syn = pcomp2_;


    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
