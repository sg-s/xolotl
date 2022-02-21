
// component info: Rectifying Electrical synapse 
// component source [Gutierrez & Marder et al. 2013](https://www.jneurosci.org/content/jneuro/33/32/13238.full.pdf)
//

#ifndef RECTIFYINGELECTRICAL
#define RECTIFYINGELECTRICAL
#include "synapse.hpp"

class RectifyingElectrical: public synapse {

public:


    // specify parameters + initial conditions
    RectifyingElectrical(double gmax_, double s_, double E_, double V_alpha)
    {
        gmax = gmax_;
        E = E_;
        s = s_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gmax)) { gmax = 0; }
        if (isnan (E)) { E = 0; }
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


double RectifyingElectrical::G_rec_inf(double V_pre, double V_alpha) {

    double V_post = post_syn->V_prev;

    // G_max is 1 and G_min is 0 in the paper,
    // so the equation reduces down to this
    return 1/(1+exp((V_pre-V_post)/(V_alpha)));
}

void RectifyingElectrical::integrate(void) {
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V_prev;

    g = gmax*G_rec_inf();


}



void RectifyingElectrical::checkSolvers(int k) {
    if (k == 0) {
        return;
    } 
    mexErrMsgTxt("[RectifyingElectrical] Unsupported solver order\n");
}

int RectifyingElectrical::getFullState(double *syn_state, int idx) {
    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}





#endif
