// nearly-instantaneous rise synaptic conductance for NMDAergic receptors
// based on the Jahr-Stevens model
// based on C. Borgers "An Introduction to Modeling Neuronal Dynamics" Ch. 20
//
// notably, this current depends on both the post-synaptic and pre-synaptic membrane potentials
// as well as an external magnesium concentration
// there are two time constants, a rise-time and a delay-time constant

#ifndef NMDA
#define NMDA
#include "synapse.hpp"
// #include "math.hpp"

class NMDAergic: public synapse {

public:

  double Mg; // Mg++ concentration in millimolar (mM)
  double u; // Mg++ block gating variable
  double tau_r; // rising-phase time constant (ms)
  double tau_d; // decaying-phase time constant (ms)

    // specify parameters + initial conditions
    NMDAergic(double gmax_, double s_, double u_, double E_, double Mg_, double tau_r_, double tau_d_)
    {
        gmax = gmax_;
        E = E_;
        s = s_;
        u = u_;
        Mg = Mg_;
        tau_r = tau_r_;
        tau_d = tau_d_;

        // defaults
        if (isnan (s)) { s = 0; }
        if (isnan(u)) { u = 0; }
        if (isnan (gmax)) { gmax = 0; }
        if (isnan (E)) { E = 0; }
        if (isnan (Mg)) { Mg = 1; }
        if (isnan (tau_r)) { tau_r = 2; }
        if (isnan (tau_d)) { tau_d = 100; }
        is_electrical = false;
    }

    // dynamics functions
    double ss_core(double);
    double s_inf(double);
    double tau_s(double);
    double sdot(double, double, double);

    // integration functions
    void checkSolvers(int);
    void integrate(void);
    void integrateMS(int, double, double);

    // output/connection functions
    int getFullStateSize(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
    int getFullState(double*, int);

};

int NMDAergic::getFullStateSize()
{
    return 3;
}

// // //
double NMDAergic::ss_core(double V_pre)
{
    return (1.0 + tanh(V_pre/10.0))/2.0;
}

double NMDAergic::s_inf(double ss)
{
    return ss / ( ss + tau_r / tau_d );
}

double NMDAergic::tau_s(double ss)
{
    return tau_r / (ss + tau_r / tau_d);
}

double NMDAergic::sdot(double V_pre, double V_post, double s_)
{
    return ss_core(V_pre) * (1 - s_) / tau_r - s_ / tau_d;
}

void NMDAergic::integrate(void)
{
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn -> V;
    double V_post = post_syn -> V;
    double ss = ss_core(V_pre);
    double sinf = s_inf(ss);

    // integrate using exponential Euler
    s = (sinf + (s - sinf)*exp(-dt/tau_s(ss)));
    u = 1.0 / (1.0 + Mg / 3.57 * exp(-0.062*V_post));
    g = gmax*s*u;
}

void NMDAergic::integrateMS(int k, double V, double Ca)
{

    double V_pre;
    double V_post;


    if (k == 0) {
        V_pre = pre_syn->V_prev;
        V_post = post_syn->V_prev;
        k_s[0] = dt*(sdot(V_pre, V_post, s));

    } else if (k == 1) {

        V_pre = pre_syn->V_prev + pre_syn->k_V[0]/2;
        V_post = post_syn->V_prev + post_syn->k_V[0]/2;
        k_s[1] = dt*(sdot(V_pre, V_post, s + k_s[0]/2));


    } else if (k == 2) {

        V_pre = pre_syn->V_prev + pre_syn->k_V[1]/2;
        V_post = post_syn->V_prev + post_syn->k_V[1]/2;
        k_s[2] = dt*(sdot(V_pre, V_post, s + k_s[1]/2));

    } else if (k == 3) {
        V_pre = pre_syn->V_prev + pre_syn->k_V[2];
        V_post = post_syn->V_prev + post_syn->k_V[2];
        k_s[3] = dt*(sdot(V_pre, V_post, s + k_s[2]));


    } else {
        // last step

        s = s + (k_s[0] + 2*k_s[1] + 2*k_s[2] + k_s[3])/6;
        u = 1.0 / (1.0 + Mg / 3.57 * exp(-0.062*V_post));

        if (s < 0) {s = 0;}
        if (s > 1) {s = 1;}
    }

}


int NMDAergic::getFullState(double *syn_state, int idx)
{
    // give it the current synapse variable
    syn_state[idx] = s;
    idx++;
    
    // give it the current magnesium block variable
    syn_state[idx] = u;
    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*u*(post_syn->V - E);
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

void NMDAergic::checkSolvers(int k){
    if (k == 0) {
        return;
    } else if (k == 4) {
        return;
    }
    mexErrMsgTxt("[NMDAergic] Unsupported solver order\n");
}


#endif
