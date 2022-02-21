
// component info: Rectifying Electrical synapse 
// component source [Gutierrez & Marder et al. 2013](https://www.jneurosci.org/content/jneuro/33/32/13238.full.pdf)
//

#pragma once
#include "synapse.hpp"

class RectifyingElectrical: public synapse {

public:


    double V_alpha;

    // specify parameters + initial conditions
    RectifyingElectrical(double gmax_, double V_alpha_)
    {
        gmax = gmax_;
        V_alpha = V_alpha_;

        // defaults
        if (isnan (gmax)) { gmax = 0; }
        is_electrical = false;
    }

    void integrate(void);
    void checkSolvers(int);

    double G_rec_inf(double, double);

    int getFullStateSize(void);
    int getFullState(double*, int);
};

int RectifyingElectrical::getFullStateSize() {
    return 0;
}


double RectifyingElectrical::G_rec_inf(double V_pre) {

    double V_post = post_syn->V_prev;

    // G_max is 1 and G_min is 0 in the paper,
    // so the equation reduces down to this
    return 1/(1+exp((V_pre-V_post)/(V_alpha)));
}

void RectifyingElectrical::integrate(void) {
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V_prev;

    g = gmax*G_rec_inf(V_pre);


}



void RectifyingElectrical::checkSolvers(int k) {
    if (k == 0) {
        return;
    } 
    mexErrMsgTxt("[RectifyingElectrical] Unsupported solver order\n");
}

int RectifyingElectrical::getFullState(double *syn_state, int idx) {
    // also return the current from this synapse
    syn_state[idx] = gmax*G_rec_inf(V_pre)*(post_syn->V - pre_syn->V);
    idx++;
    return idx;
}




