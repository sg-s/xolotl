// Cholingeric Synapse 
#ifndef CHOLINERGIC
#define CHOLINERGIC
#include "../../synapse.hpp"

class Cholinergic: public synapse {

public:

    // specify parameters + initial conditions 
    Cholinergic(double g_, double s_)
    {
        gbar = g_;
        E = -80.0;
        Delta = 5.0;
        Vth = -35.0;
        k_ = 0.01;
        s = s_;
    }
    
    void integrate(double dt);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
};

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

double Cholinergic::getCurrent(double V_post)
{   
    double I_out = -gbar*s*(V_post-E)/1000.0;
    // mexPrintf("I_out = %f\n",I_out);
    return I_out;// in units of nA
}

void Cholinergic::connect(compartment *pcomp1_, compartment *pcomp2_) 
{
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
