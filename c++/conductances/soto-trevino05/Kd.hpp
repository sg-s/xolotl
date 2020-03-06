// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Delayed Potassium
// http://jn.physiology.org/content/94/1/590.short
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

        p = 4;

        // allow this channel to be approximated
        approx_m = 1;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }

    }


    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);
};

string Kd::getClass(){return "Kd";}

double Kd::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+14.2)/11.8));}
double Kd::tau_m(double V, double Ca) {return 7.2 - 6.4/(1.0+exp(-(V+28.3)/19.2));}


#endif
