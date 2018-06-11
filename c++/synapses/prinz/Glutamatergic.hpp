// GLUTAMATERGIC Synapse 
#ifndef GLUTAMATERGIC
#define GLUTAMATERGIC
#include "synapse.hpp"

class Glutamatergic: public synapse {

public:

    // specify parameters + initial conditions 
    Glutamatergic(double g_, double s_)
    {
        gbar = g_;
        E = -70.0;
        Delta = 5.0;
        Vth = -35.0;
        k_ = 0.025;
        s = s_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gbar)) { gbar = 0; }
        is_electrical = false;

    }
    
    void integrate(double dt);
    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    int getFullState(double*, int);
};

void Glutamatergic::integrate(double dt)
{   
    
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;

    // find s_inf 
    double s_inf = 1.0/(1.0+exp((Vth - V_pre)/Delta));

    // integrate using exponential Euler 
    double tau_s = (1.0 - s_inf)/k_;

    s = s_inf + (s - s_inf)*exp(-dt/tau_s);

    

}


int Glutamatergic::getFullState(double *syn_state, int idx)
{
    // give it the current synapse variable
    syn_state[idx] = s;
    idx++;

    // also return the current from this synapse
    syn_state[idx] = gbar*s*(post_syn->V - E);
    idx++;
    return idx;
}


int Glutamatergic::getFullStateSize()
{
    return 2; 
}


void Glutamatergic::connect(compartment *pcomp1_, compartment *pcomp2_) 
{
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 


    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
