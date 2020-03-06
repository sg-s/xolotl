// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// inward rectifying potassium conductance
// 
#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

public:

    //specify both gbar and erev and initial conditions
    Kd(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -77; }

        unitary_conductance = 20e-6; // uS

        p = 4;

        // allow this channel to be approximated
        approx_m = 1;
    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

    double Alpha(double);
    double Beta(double);

};

string Kd::getClass(){
    return "Kd";
}


double Kd::Alpha(double V) {
    return 0.01 * (10-V) / (exp((10-V)/10)-1);
}

double Kd::Beta(double V) {
    return 0.125 * exp(-V/80);
}

double Kd::m_inf(double V, double Ca) {return Alpha(V)/(Alpha(V)+Beta(V));}
double Kd::tau_m(double V, double Ca) {return 1.0/(Alpha(V)+Beta(V));}


#endif
