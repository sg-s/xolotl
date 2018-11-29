// Cholingeric Synapse
#ifndef CHOLINERGIC
#define CHOLINERGIC
#include "synapse.hpp"

class Cholinergic: public synapse {

public:

    double Delta = 5.0;
    double k_ = 0.01;
    double Vth = -35.0;


    // specify parameters + initial conditions
    Cholinergic(double g_, double s_)
    {
        gmax = g_;
        E = -80.0;


        // dynamic variables
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

    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
    int getFullState(double*, int);
};

int Cholinergic::getFullStateSize()
{
    return 2;
}


double Cholinergic::s_inf(double V_pre)
{
    return 1.0/(1.0+exp((Vth - V_pre)/Delta));
}

double Cholinergic::tau_s(double sinf_)
{
    return (1 - sinf_)/k_;
}

double Cholinergic::sdot(double V_pre, double s_)
{
    double sinf = s_inf(V_pre);
    return (sinf - s_)/tau_s(sinf);
}

void Cholinergic::integrate(void)
{
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn->V;
    double sinf = s_inf(V_pre);

    // integrate using exponential Euler
    s = sinf + (s - sinf)*exp(-dt/tau_s(sinf));

    g = gmax*s;


}

void Cholinergic::integrateMS(int k, double V, double Ca)
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

void Cholinergic::checkSolvers(int k){
    if (k == 0) {
        return;
    } else if (k == 4) {
        return;
    }
    mexErrMsgTxt("[Cholinergic] Unsupported solver order\n");
}

int Cholinergic::getFullState(double *syn_state, int idx)
{
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
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
