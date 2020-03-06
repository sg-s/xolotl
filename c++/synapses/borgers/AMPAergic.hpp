// nearly-instantaneous rise synaptic conductance for AMPAergic receptors
// based on C. Borgers "An Introduction to Modeling Neuronal Dynamics" Ch. 20
#ifndef AMPA
#define AMPA
#include "synapse.hpp"
// #include "math.hpp"

class AMPAergic: public synapse {

public:

    double tau_r;
    double tau_d;

      // specify parameters + initial conditions
      AMPAergic(double gmax_, double s_, double E_, double tau_r_, double tau_d_)
      {
          gmax = gmax_;
          E = E_;
          s = s_;
          tau_r = tau_r_;
          tau_d = tau_d_;

          // defaults
          if (isnan (s)) { s = 0; }
          if (isnan (gmax)) { gmax = 0; }
          if (isnan (E)) { E = 0; }
          if (isnan (tau_r)) { tau_r = 1; }
          if (isnan (tau_d)) { tau_d = 1; }
          is_electrical = false;
      }

      // dynamics functions
      double ss_core(double);
      double s_inf(double);
      double tau_s(double);
      double sdot(double, double);

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

int AMPAergic::getFullStateSize()
{
    return 2;
}

double AMPAergic::ss_core(double V_pre)
{
    return (1.0 + tanh(V_pre/10.0))/2.0;
}

double AMPAergic::s_inf(double ss)
{
    return ss / ( ss + tau_r / tau_d );
}

double AMPAergic::tau_s(double ss)
{
    return tau_r / (ss + tau_r / tau_d);
}

double AMPAergic::sdot(double V_pre, double s_)
{
    return ss_core(V_pre) * (1 - s_) / tau_r - s_ / tau_d;
}

void AMPAergic::integrate(void)
{
    // figure out the voltage of the pre-synaptic neuron
    double V_pre = pre_syn -> V;
    double ss = ss_core(V_pre);
    double sinf = s_inf(ss);

    // integrate using exponential Euler
    s = sinf + (s - sinf)*exp(-dt/tau_s(ss));
    g = gmax*s;
}

void AMPAergic::integrateMS(int k, double V, double Ca)
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

int AMPAergic::getFullState(double *syn_state, int idx)
{
    // give it the current synapse variable
    syn_state[idx] = s;

    idx++;

    // also return the current from this synapse
    syn_state[idx] = gmax*s*(post_syn->V - E);
    idx++;
    return idx;
}

void AMPAergic::connect(compartment *pcomp1_, compartment *pcomp2_)
{
    pre_syn = pcomp1_;
    post_syn = pcomp2_;

    // tell the post-synaptic cell that we're connecting to it
    post_syn->addSynapse(this);
}

void AMPAergic::checkSolvers(int k){
    if (k == 0) {
        return;
    } else if (k == 4) {
        return;
    }
    mexErrMsgTxt("[NMDAergic] Unsupported solver order\n");
}


#endif
