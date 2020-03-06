// xolotl
//
// delayed rectifier potassium current
// Giovannini et al. 2015
// https://hal.archives-ouvertes.fr/hal-01426362/file/Giovannini-ICAN-Hippocampus_submittedRev.pdf

#ifndef KD
#define KD
#include "conductance.hpp"

// inherit conductance class spec
class Kd: public conductance {

public:

  // specify parameters & initial conditions
  Kd(double gbar_, double E_, double m_)
  {
    gbar = gbar_;
    E = E_;
    m = m_;

    p = 4;

    // allow this channel to be approximated?
    approx_m = 1;

    // defaults
    if (isnan(gbar)) { gbar = 0; }
    if (isnan(E)) { E = -80; }
  }

  double m_inf(double, double);
  double tau_m(double, double);
  double a_m(double, double);
  double b_m(double, double);
  string getClass(void);

};

string Kd::getClass()
{
  return "Kd";
}

double Kd::a_m(double V, double Ca) {return 0.032 * (15 - V + 55) / (exp((15 - V + 55)/5) - 1);}
double Kd::b_m(double V, double Ca) {return 0.5 * exp((10 - V + 55)/40);}

double Kd::m_inf(double V, double Ca) {return a_m(V, Ca) / (a_m(V, Ca) + b_m(V, Ca));}
double Kd::tau_m(double V, double Ca) {return 1 / (a_m(V, Ca) + b_m(V, Ca));}

#endif
