// inhibitory synapse from the leech heartbeat pacemaker
// Journal of Neuroscience 15 December 2002, 22 (24) 10580-10592;
// DOI: https://doi-org.resources.library.brandeis.edu/10.1523/JNEUROSCI.22-24-10580.2002
// http://www.jneurosci.org/content/22/24/10580.short


#ifndef INHIBITORY
#define INHIBITORY
#include "synapse.hpp"

class Inhibitory: public synapse {

public:

    double tau_1 = 2.0;
    double tau_2 = 11.0;
    double m = 0.0;
    double y = 0.0;
    double x = 0.0;

    // specify parameters + initial conditions
    Inhibitory(double gmax_, double s_, double m_, double y_, double x_, double tau_1_, double tau_2_, double E_)
    {

        // wire up inputs
        gmax    = gmax_;
        s       = s_;
        tau_1   = tau_1_;
        tau_2   = tau_2_;
        m       = m_;
        y       = y_;
        x       = x_;
        E       = E_;

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

    double x_inf(double);
    double m_inf(double);
};

int Inhibitory::getFullStateSize() {
    return 2;
}

void Inhibitory::integrate(void) {
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;

    m = m_inf(V_pre) + (m - m_inf(V_pre))*exp(-dt/0.2);
    y = x + (y - x)*exp(-dt/tau_1);
    x = x_inf(V_pre) + (x - x_inf(V_pre))*exp(-dt/tau_2); // do last b/c y depends on x

    s = m*y;
    g = gmax*s;

}


void Inhibitory::checkSolvers(int k){
    if (k == 0) {
        return;
    }
    mexErrMsgTxt("[Inhibitory] Unsupported solver order\n");
}

int Inhibitory::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = g*(post_syn->V - E);
    idx++;
    return idx;
}

void Inhibitory::connect(compartment *pcomp1_, compartment *pcomp2_) {
    pre_syn = pcomp1_;
    post_syn = pcomp2_;

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}


// steady-state functions
double Inhibitory::m_inf(double Vpre) {return 1.0 / (1.0 + exp(-Vpre-0.01));}
double Inhibitory::x_inf(double Vpre) {return 0.1 + 0.9 / (1.0 + exp(-Vpre-0.04));}


#endif
