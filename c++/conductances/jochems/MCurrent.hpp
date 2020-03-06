// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// M-current (potassium)
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4406621/
// Jochems & Yoshida 2015
#ifndef MCURRENT
#define MCURRENT
#include "conductance.hpp"

// inherit conductance class spec
class MCurrent: public conductance {

public:

  // specify parameters & initial conditions
  MCurrent(double gbar_, double E_, double m_)
  {
    gbar = gbar_;
    E = E_;
    m = m_;

    p = 1;

    // allow this channel to be approximated?
    approx_m = 1;

    // defaults
    if (isnan(gbar)) { gbar = 0; }
    if (isnan(E)) { E = -100; }
  }

  double m_inf(double, double);
  double tau_m(double, double);
  string getClass(void);

};

string MCurrent::getClass()
{
  return "MCurrent";
}

double MCurrent::m_inf(double V, double Ca) {return 1 / (1 + exp((-(V + 35)/10)));}
double MCurrent::tau_m(double V, double Ca) {return 4 / (3.3 * exp((V + 35)/20) + exp(-(V + 35)/20));}

#endif
