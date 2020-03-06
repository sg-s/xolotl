// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// calcium-activated nonspecific current
// Giovannini et al. 2015
// https://hal.archives-ouvertes.fr/hal-01426362/file/Giovannini-ICAN-Hippocampus_submittedRev.pdf

#ifndef CAN
#define CAN
#include "conductance.hpp"

//inherit conductance class spec
class CaN: public conductance {

public:

    // time constant (ms)
    double beta;
    // Q10 value, unitless
    double Q10;
    // temperature adjustment factor (unitless)
    double T_adj;
    // fudge-factor for calcium concentration (uM)
    double Ca_c;

    // specify parameters + initial conditions
    CaN(double gbar_, double E_, double m_, double beta_, double Q10_, double Ca_c_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        beta = beta_;
        Q10 = Q10_;
        Ca_c = Ca_c_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -20; }
        if (isnan(beta)) { beta = 0.00002; }
        if (isnan(Q10)) { Q10 = 3.0; }
        if (isnan(Ca_c)) { Ca_c = 1.0; }

        p = 2;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        // save the adjusted temperature scaling
        T_adj = pow(Q10, (temperature - temperature_ref) / 10);

    }

    double m_inf(double, double);
    double tau_m(double, double);
    double a_m(double, double);
    double b_m(double, double);
    double alpha(double);
    string getClass(void);

};

string CaN::getClass(){
    return "CaN";
}

double CaN::alpha(double Ca) {return beta * fast_pow(Ca / Ca_c, 2); }

double CaN::a_m(double V, double Ca) {return alpha(Ca) * T_adj;}
double CaN::b_m(double V, double Ca) {return beta * T_adj;}

double CaN::m_inf(double V, double Ca) {return a_m(V, Ca) / (a_m(V, Ca) + b_m(V, Ca));}
double CaN::tau_m(double V, double Ca) {return 1 / (a_m(V, Ca) + b_m(V, Ca));}


#endif
