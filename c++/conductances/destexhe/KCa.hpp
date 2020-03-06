// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// C++ dependent potassium current
// Destexhe, Contreras, Sejnowski, & Steriade 1994
// https://www.ncbi.nlm.nih.gov/pubmed/7527077

#ifndef KCA
#define KCA
#include "conductance.hpp"

//inherit conductance class spec
class KCa: public conductance {

public:

    // tuning parameters for the kinetics
    double alpha;
    double beta;

    // specify parameters + initial conditions
    KCa(double gbar_, double E_, double m_, double alpha_, double beta_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // tuning parameters for the kinetics
        alpha = alpha_;
        beta = beta_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -85; }
        if (isnan (alpha)) { alpha = 48; }
        if (isnan (beta)) { beta = 0.03; }

        p = 2;

        // allow approximations
        approx_m = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string KCa::getClass(){
    return "KCa";
}

double KCa::m_inf(double V, double Ca) {return alpha * fast_pow(Ca, 2) / (alpha * fast_pow(Ca, 2) + beta);}
double KCa::tau_m(double V, double Ca) {return 1 / (alpha * fast_pow(Ca, 2) + beta);}


#endif
