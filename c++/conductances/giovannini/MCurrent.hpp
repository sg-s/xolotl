// xolotl
//
// M-current based on Yamada et al. 1989
// Giovannini et al. 2015
// https://hal.archives-ouvertes.fr/hal-01426362/file/Giovannini-ICAN-Hippocampus_submittedRev.pdf

#ifndef MCURRENT
#define MCURRENT
#include "conductance.hpp"

// inherit conductance class spec
class MCurrent: public conductance {

public:

  // time constant (ms)
  double tau;

  // specify parameters & initial conditions
  MCurrent(double gbar_, double E_, double m_, double h_, double tau_)
  {
    gbar = gbar_;
    E = E_;
    m = m_;
    h = h_;
    tau = tau_; // ms

    p = 1;

    // allow this channel to be approximated?
    approx_m = 1;
    approx_h = 1;

    // defaults
    if (isnan(gbar)) { gbar = 0; }
    if (isnan(E)) { E = -80; }
    if (isnan(tau)) { tau = 1000; }
  }

  double m_inf(double, double);
  double tau_m(double, double);
  double a_m(double, double);
  double b_m(double, double);
  string getClass(void);

};

string MCurrent::getClass()
{
  return "MCurrent";
}

double MCurrent::a_m(double V, double Ca) {return (1 / tau) * (3.3 * exp((V + 35)/20) + exp(-(V + 35)/20)) / exp((-35 + V)/10) + 1;}
double MCurrent::b_m(double V, double Ca) {return (1 / tau) * (3.3 * exp((V + 35)/20) + exp(-(V + 35)/20) * (1 - 1 / (exp((-35 + V)/10) + 1)));}

double MCurrent::m_inf(double V, double Ca) {return a_m(V, Ca) / (a_m(V, Ca) + b_m(V, Ca));}
double MCurrent::tau_m(double V, double Ca) {return 1 / (a_m(V, Ca) + b_m(V, Ca));}

#endif
