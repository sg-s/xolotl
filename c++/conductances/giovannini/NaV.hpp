// xolotl
//
// fast sodium current
// Giovannini et al. 2015
// https://hal.archives-ouvertes.fr/hal-01426362/file/Giovannini-ICAN-Hippocampus_submittedRev.pdf

#ifndef NAV
#define NAV
#include "conductance.hpp"

// inherit conductance class spec
class NaV: public conductance {

public:

  // specify parameters & initial conditions
  NaV(double gbar_, double E_, double m_, double h_)
  {
    gbar = gbar_;
    E = E_;
    m = m_;
    h = h_;

    p = 3;
    q = 1;

    // allow this channel to be approximated?
    approx_m = 1;
    approx_h = 1;

    // defaults
    if (isnan(gbar)) { gbar = 0; }
    if (isnan(E)) { E = 50; }
  }

  double m_inf(double, double);
  double h_inf(double, double);
  double tau_m(double, double);
  double tau_h(double, double);
  double a_m(double, double);
  double b_m(double, double);
  double a_h(double, double);
  double b_h(double, double);
  string getClass(void);

};

string NaV::getClass()
{
  return "NaV";
}

double NaV::a_m(double V, double Ca) {return 0.32 * (13 - V + 55) / (exp((13 - V + 55)/4) - 1);}
double NaV::a_h(double V, double Ca) {return 0.128 * exp((17 - V + 55)/18);}
double NaV::b_m(double V, double Ca) {return 0.28 * (V - 55 - 40) / (exp((V - 55 - 40)/5) - 1);}
double NaV::b_h(double V, double Ca) {return 4 / (exp((40 - V + 55)/5) - 1);}

double NaV::m_inf(double V, double Ca) {return a_m(V, Ca) / (a_m(V, Ca) + b_m(V, Ca));}
double NaV::h_inf(double V, double Ca) {return a_h(V, Ca) / (a_h(V, Ca) + b_h(V, Ca));}
double NaV::tau_m(double V, double Ca) {return 1 / (a_m(V, Ca) + b_m(V, Ca));}
double NaV::tau_h(double V, double Ca) {return 1 / (a_h(V, Ca) + b_h(V, Ca));}

#endif
