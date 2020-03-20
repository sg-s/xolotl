// component info: Graded Glutamatergic synapse 
// component source [Prinz et al. 2004](https://www.nature.com/articles/nn1352)
//

#ifndef GLUTAMATERGIC
#define GLUTAMATERGIC
#include "synapse.hpp"

class Glutamatergic: public synapse {

public:


    double Delta;

    double k_;
    double Vth;


    // specify parameters + initial conditions 
    Glutamatergic(double gmax_, double s_)
    {
        gmax = gmax_;
        E = -70.0;
        Delta = 5.0;
        Vth = -35.0;
        k_ = 0.025;
        s = s_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gmax)) { gmax = 0; }
        is_electrical = false;

    }
    
    void integrate(void);
    void integrateMS(int, double, double);
    void checkSolvers(int);

    double s_inf(double);
    double tau_s(double);
    double sdot(double, double);
    
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    int getFullState(double*, int);
    int getFullStateSize(void);
};

int Glutamatergic::getFullStateSize()
{
    return 2; 
}


double Glutamatergic::s_inf(double V_pre) {
    return 1.0/(1.0+exp((Vth - V_pre)/Delta));
}

double Glutamatergic::tau_s(double sinf_) {
    return (1 - sinf_)/k_;
}

double Glutamatergic::sdot(double V_pre, double s_) {
    double sinf = s_inf(V_pre);
    return (sinf - s_)/tau_s(sinf);
}

void Glutamatergic::integrate(void) {   
    // figure out the voltage of the pre-synaptic neuron
    

    double V_pre = pre_syn->V;
    double sinf = s_inf(V_pre);

    // integrate using exponential Euler
    s = sinf + (s - sinf)*exp(-dt/tau_s(sinf));

    g = gmax*s;
    
}

void Glutamatergic::integrateMS(int k, double V, double Ca) {

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

void Glutamatergic::checkSolvers(int k){
    if (k == 0) {
        return;
    } else if (k == 4) {
        return;
    }
    mexErrMsgTxt("[Glutamatergic] Unsupported solver order\n");
}

int Glutamatergic::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}

void Glutamatergic::connect(compartment *pcomp1_, compartment *pcomp2_) {
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
