// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// low-threshold calcium conductance
// Giovannini et al. 2015
// https://hal.archives-ouvertes.fr/hal-01426362/file/Giovannini-ICAN-Hippocampus_submittedRev.pdf

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
    double a_m(double, double);
    double b_m(double, double);
    double a_h(double, double);
    double b_h(double, double);
    string getClass(void);


};

string CaT::getClass(){
    return "CaT";
}

double CaT::a_m(double V, double Ca) {return 0.055 * (-27 - V) / (exp((-27 - V)/3.8) - 1);}
double CaT::a_h(double V, double Ca) {return 0.000457 * exp((-13 - V)/50);}
double CaT::b_m(double V, double Ca) {return 0.94 * exp((-75 - V)/17);}
double CaT::b_h(double V, double Ca) {return 0.0065 * (exp((-15 - V)/28) + 1);}

double CaT::m_inf(double V, double Ca) {return a_m(V, Ca) / (a_m(V, Ca) + b_m(V, Ca));}
double CaT::h_inf(double V, double Ca) {return a_h(V, Ca) / (a_h(V, Ca) + b_h(V, Ca));}
double CaT::tau_m(double V, double Ca) {return 1 / (a_m(V, Ca) + b_m(V, Ca));}
double CaT::tau_h(double V, double Ca) {return 1 / (a_h(V, Ca) + b_h(V, Ca));}


#endif
