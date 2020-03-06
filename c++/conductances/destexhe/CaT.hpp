// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// low-threshold Ca++ current
// Destexhe, Contreras, Sejnowski, & Steriade 1994
// https://www.ncbi.nlm.nih.gov/pubmed/7527077
//
// These kinetics correspond to a temperature of 36 deg. C
// assuming Q10 values of 5 and 3
// and an extracellular Ca++ concentration of 2 mM

#ifndef CAT
#define CAT
#include "conductance.hpp"

//inherit conductance class spec
class CaT: public conductance {

public:

    // specify parameters + initial conditions
    CaT(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = 30; }

        p = 2;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        is_calcium = true;

    }


    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string CaT::getClass(){
    return "CaT";
}

double CaT::m_inf(double V, double Ca) {return 1 / (1 + exp(-(V + 52)/7.4));}
double CaT::h_inf(double V, double Ca) {return 1 / (1 + exp((V + 80)/5));}
double CaT::tau_m(double V, double Ca) {return 0.44 + 0.15 / (exp((V + 27)/10) + exp(-(V + 102)/15));}
double CaT::tau_h(double V, double Ca) {return 22.7 + 0.27 / (exp((V + 48)/4) + exp((-(V + 407)/50)));}


#endif
