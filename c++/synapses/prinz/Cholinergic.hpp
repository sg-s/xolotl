// Cholingeric Synapse 
#ifndef CHOLINERGIC
#define CHOLINERGIC
#include "synapse.hpp"

class Cholinergic: public synapse {

public:

    double Delta;
    double k_;
    double Vth;


    // specify parameters + initial conditions 
    Cholinergic(double g_, double s_)
    {
        gbar = g_;
        E = -80.0;
        Delta = 5.0;
        Vth = -35.0;
        k_ = 0.01;
        s = s_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gbar)) { gbar = 0; }
        is_electrical = false;
    }
    
    void integrate(double dt);
    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
    int getFullState(double*, int);
};

int Cholinergic::getFullStateSize()
{
    return 2; 
}

void Cholinergic::integrate(double dt)
{   
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;

    // find s_inf 
    double s_inf = 1.0/(1.0+exp((Vth - V_pre)/Delta));

    // integrate using exponential Euler 
    double tau_s = (1 - s_inf)/k_;
    
    s = s_inf + (s - s_inf)*exp(-dt/tau_s);
    
}


int Cholinergic::getFullState(double *syn_state, int idx)
{
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = gbar*s*(post_syn->V - E);
    idx++;
    return idx;
}

void Cholinergic::connect(compartment *pcomp1_, compartment *pcomp2_) 
{
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
