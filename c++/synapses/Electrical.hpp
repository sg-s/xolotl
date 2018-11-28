// One-way electrical Synapse
// For symmetric electrical synapses, connect two copies
// of this in opposite polarities
#ifndef ELECTRICAL
#define ELECTRICAL
#include "synapse.hpp"

class Electrical: public synapse {

public:

    // specify parameters + initial conditions
    Electrical(double g_)
    {
        gmax = g_;
        s = 1; // always active
        is_electrical = true;
    }

    void integrate(double dt);
    void connect(compartment *pcomp1_, compartment *pcomp2_);

    int getFullState(double*, int);
    int getFullStateSize(void);
};

void Electrical::integrate(void)
{
    // set E to V_pre
    E = (pre_syn->V);
}

int Electrical::getFullStateSize()
{
    return 0;
}

int Electrical::getFullState(double *syn_state, int idx)
{
    return idx;
}



void Electrical::connect(compartment *pcomp1_, compartment *pcomp2_)
{
    pre_syn = pcomp1_;
    post_syn = pcomp2_;

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}

#endif
