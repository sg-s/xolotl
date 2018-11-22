// nearly-instantaneous rise synaptic conductance for NMDAergic receptors
// based on the Jahr-Stevens model
// based on C. Borgers "An Introduction to Modeling Neuronal Dynamics" Ch. 20
#ifndef NMDA
#define NMDA
#include "synapse.hpp"
#include "math.hpp"

class NMDAergic: public synapse {

public:

  double Mg; // Mg++ concentration in millimolar (mM)

    // specify parameters + initial conditions
    NMDAergic(double g_, double s_, double E_, double Mg_)
    {
        gbar = g_;
        E = E_;
        s = s_;
        Mg = Mg_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gbar)) { gbar = 0; }
        if (isnan (E)) { E = 0; }
        if (isnan (Mg)) { Mg = 1; }
        is_electrical = false;
    }

    void integrate(double dt);
    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
    int getFullState(double*, int);
};

int NMDAergic::getFullStateSize()
{
    return 2;
}

void NMDAergic::integrate(double dt)
{
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;
    double V_post = post_syn->V;

    // find s_inf
    double s_inf = ((1.0 + tanh(V_pre/10.0))/2.0) / ( ((1.0 + tanh(V_pre/10.0))/2.0) + tau_r / tau_d );

    // integrate using exponential Euler
    double tau_s = tau_r / ( ((1.0 + tanh(V_pre/10.0))/2.0) + tau_r / tau_d );

    s = s_inf + (s - s_inf)*exp(-dt/tau_s) * 1.0 / (1.0 + Mg / 3.57 * exp(-0.062*V_post));

}


int NMDAergic::getFullState(double *syn_state, int idx)
{
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = gbar*s*(post_syn->V - E);
    idx++;
    return idx;
}

void NMDAergic::connect(compartment *pcomp1_, compartment *pcomp2_)
{
    pre_syn = pcomp1_;
    post_syn = pcomp2_;

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
