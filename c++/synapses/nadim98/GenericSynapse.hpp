// GENERICSYNAPSE Synapse 
#ifndef GENERICSYNAPSE
#define GENERICSYNAPSE
#include "synapse.hpp"

class GenericSynapse: public synapse {

public:


    double k;
    double Vk;
    double tau3;
    double tau4;
    double lambda;
    double Vlambda;


    // specify parameters + initial conditions 
    GenericSynapse(double gmax_, double s_, double k_, double Vk_, double tau3_, double tau4_, double lambda_, double Vlambda_, double E_)
    {
        gmax = gmax_;
        k = k_;
        Vk = Vk_;
        tau3 = tau3_;
        tau4 = tau4_;
        lambda = lambda_;
        Vlambda = Vlambda_;
        E = E_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan (gmax)) { gmax = 0; }
        if (isnan (gmax)) { gmax = -80; }
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

int GenericSynapse::getFullStateSize() {
    return 2; 
}


double GenericSynapse::s_inf(double V_pre) {
    return 1.0/(1.0+exp((V_pre - Vk)*k));
}

double GenericSynapse::tau_s(double V_pre) {
    return tau3 + tau4/(1 + exp(lambda*(V_pre - Vlambda)));
}

double GenericSynapse::sdot(double V_pre, double s_) {
    return (s_inf(V_pre) - s_)/tau_s(V_pre);
}

void GenericSynapse::integrate(void) {   
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;
    double sinf = s_inf(V_pre);

    // integrate using exponential Euler
    s = sinf + (s - sinf)*exp(-dt/tau_s(sinf));

    g = gmax*s;
    
}

void GenericSynapse::integrateMS(int k, double V, double Ca) {

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

void GenericSynapse::checkSolvers(int k){
    if (k == 0) {
        return;
    } else if (k == 4) {
        return;
    }
    mexErrMsgTxt("[GenericSynapse] Unsupported solver order\n");
}

int GenericSynapse::getFullState(double *syn_state, int idx) {
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}

void GenericSynapse::connect(compartment *pcomp1_, compartment *pcomp2_)  {
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}




#endif
