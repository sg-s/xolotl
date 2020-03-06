// Neurite to Neurite Synapse
#ifndef NEURITESYN
#define NEURITESYN
#include "synapse.hpp"

class NeuriteSyn: public synapse {

public:

    double Delta;
    double Vth;
    double k_;

    // specify parameters + initial conditions
    NeuriteSyn(double gmax_, double s_)
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

    }

    void integrate(void);

    void connect(compartment *pcomp1_, compartment *pcomp2_);
    int getFullState(double*, int);
    int getFullStateSize(void);
};

void NeuriteSyn::integrate(void) {

    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;

    // find s_inf
    double s_inf = 1.0/(1.0+exp((Vth - V_pre)/Delta));

    // integrate using exponential Euler
    double tau_s = 100.0/(1.0+exp((Vth - V_pre)/k_));

    s = s_inf + (s - s_inf)*exp(-dt/tau_s);

    g = gmax*s;

}


int NeuriteSyn::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;
    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}


int NeuriteSyn::getFullStateSize() {
    return 2;
}


void NeuriteSyn::connect(compartment *pcomp1_, compartment *pcomp2_) {
    pre_syn = pcomp1_;
    post_syn = pcomp2_;


    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
