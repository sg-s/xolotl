// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Ca++ dependent nonspecific cation current
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4406621/
// Jochems & Yoshida 2015
#ifndef CAN
#define CAN
#include "conductance.hpp"

//inherit conductance class spec
class CaN: public conductance {

protected:
    double tau_adj = 0;

public:

    // fudge-factor for scaling calcium concentration (1 uM in Destexhe et al. 1993)
    double Ca_c; // uM
    // rate constant
    double beta = 0.002; // 1/ms

    //specify both gbar and erev and initial conditions
    CaN(double gbar_, double E_, double m_, double Ca_c_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        Ca_c = Ca_c_;

        // compute the temperature scaling factor
        tau_adj = pow(3.0, (temperature - 22)/10); // unitless

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -20; }
        if (isnan(Ca_c)) { Ca_c = 0.75; }

        p = 2;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    double alpha(double);
    string getClass(void);

};

string CaN::getClass(){return "CaN";}

double CaN::alpha(double Ca) { return beta + fast_pow(Ca / Ca_c, 2); }

double CaN::m_inf(double V, double Ca) {return alpha(Ca) / (alpha(Ca) + beta);}
double CaN::tau_m(double V, double Ca) {return tau_adj / (alpha(Ca) + beta);}

#endif
