// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// C++ dependent nonspecific cation current
// Destexhe, Contreras, Sejnowski, & Steriade 1994
// https://www.ncbi.nlm.nih.gov/pubmed/7527077

#ifndef CAN
#define CAN
#include "conductance.hpp"

//inherit conductance class spec
class CaN: public conductance {

public:

    // tuning parameters for the kinetics
    double alpha;
    double beta;

    // specify parameters + initial conditions
    CaN(double gbar_, double E_, double m_, double alpha_, double beta_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // tuning parameters for the kinetics
        alpha = alpha_;
        beta = beta_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -20; }
        if (isnan (alpha)) { alpha = 20; }
        if (isnan (beta)) { beta = 0.002; }

        p = 2;

        // allow approximations
        approx_m = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string CaN::getClass(){
    return "CaN";
}

double CaN::m_inf(double V, double Ca) {return alpha * fast_pow(Ca, 2) / (alpha * fast_pow(Ca, 2) + beta);}
double CaN::tau_m(double V, double Ca) {return 1 / (alpha * fast_pow(Ca, 2) + beta);}


#endif
