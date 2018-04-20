// One-way electrical Synapse 
// For symmetric electrical synapses, connect two copies
// of this in opposite polarities 
#ifndef ELECTRICAL
#define ELECTRICAL
#include "../synapse.hpp"

class Electrical: public synapse {

public:

    // specify parameters + initial conditions 
    Electrical(double g_)
    {
        gbar = g_;
        s = 1; // always active
    }
    
    void integrate(double dt);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
};

void Electrical::integrate(double dt)
{       
    // set E to V_pre
    E = (pre_syn->V);
}

double Electrical::getCurrent(double V_post)
{   
    double V_pre = pre_syn->V;
    double I_out = gbar*(V_pre-V_post)/1000.0; // in units of nA
    return I_out;

}

void Electrical::connect(compartment *pcomp1_, compartment *pcomp2_) 
{
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}

#endif
