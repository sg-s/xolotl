// Alpha Synapse 
// in this simple model of a synapse
// the post-synapstic conductance 
// varies as an alpha function
// following every spike
#ifndef ALPHASYNAPSE
#define ALPHASYNAPSE
#include "synapse.hpp"

class AlphaSynapse: public synapse {

private:
    double time_since_spike = 0;

public:

    double tau_s = 10;
    double V_thresh = 10;


    // specify parameters + initial conditions 
    AlphaSynapse(double g_, double s_, double tau_s_, double V_thresh_, double E_)
    {

        // wire up inputs
        gmax = g_;
        s = s_;
        tau_s = tau_s_;
        V_thresh = V_thresh_;
        E = E_;
        

        time_since_spike = 0;


        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (E)) { E = 0; }
        if (isnan (gmax)) { gmax = 0; }
        is_electrical = false;
    }
    
    void integrate(void);
    void checkSolvers(int);

    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
    int getFullState(double*, int);
};

int AlphaSynapse::getFullStateSize() {
    return 2; 
}



void AlphaSynapse::integrate(void) {   
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;

    if (V_pre > V_thresh) {
        // reset timer
        time_since_spike = 0;
    } else {
        time_since_spike += dt;
    }

    s = (time_since_spike/tau_s)*exp(1-(time_since_spike/tau_s));
    g = gmax*s;

}


void AlphaSynapse::checkSolvers(int k){
    if (k == 0) {
        return;
    } 
    mexErrMsgTxt("[AlphaSynapse] Unsupported solver order\n");
}

int AlphaSynapse::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = g*(post_syn->V - E);
    idx++;
    return idx;
}

void AlphaSynapse::connect(compartment *pcomp1_, compartment *pcomp2_) {
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
