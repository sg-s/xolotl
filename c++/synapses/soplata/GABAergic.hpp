// XOLOTL

// GABAergic (A) synapse

// Reference:
//   "Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N.
//   J. (2010). Thalamocortical model for a propofol-induced alpha-rhythm
//   associated with loss of consciousness. Proceedings of the National Academy of
//   Sciences, 107(52), 22665-22670. doi:10.1073/pnas.1017069108"

#ifndef GABA
#define GABA
#include "synapse.hpp"

class GABAergic: public synapse {

public:


    // specify parameters + initial conditions
    GABAergic(double g_, double s_, double E_)
    {
        gmax = g_;
        E = E_;
        s = s_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gmax)) { gmax = 0; }
        if (isnan (E)) { E = 0; }
        is_electrical = false;
    }

    void integrate(void);
    void integrateMS(int, double, double);
    void checkSolvers(int);

    double sdot(double, double);

    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
    int getFullState(double*, int);
};

int GABAergic::getFullStateSize()
{
    return 2;
}


double GABAergic::sdot(double V_pre, double s_)
{
    return 2.0*(1.0 + tanh(V_pre/4.0))*(1.0-s_) - s_/5.0;
}

void GABAergic::integrate(void)
{
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;

    // integrate using forward Euler
    s = s + dt*sdot(V_pre, s);

    g = gmax*s;


}

void GABAergic::integrateMS(int k, double V, double Ca)
{

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

void GABAergic::checkSolvers(int k){
    if (k == 0) {
        return;
    } else if (k == 4) {
        return;
    }
    mexErrMsgTxt("[GABAergic] Unsupported solver order\n");
}

int GABAergic::getFullState(double *syn_state, int idx)
{
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}

void GABAergic::connect(compartment *pcomp1_, compartment *pcomp2_)
{
    pre_syn = pcomp1_;
    post_syn = pcomp2_;

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
