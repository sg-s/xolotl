// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Calcium-Gated Potassium Conductance
// http://www.jneurosci.org/content/32/32/10995
// Kispersky, Caplan, Marder 2012
#ifndef KCA
#define KCA
#include "conductance.hpp"

//inherit conductance class spec
class KCa: public conductance {

public:


    //specify both gbar and erev and initial conditions
    KCa(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = 0; }

        p = 4;

    }

    double m_inf(double V, double Ca);
    double tau_m(double, double);
    string getClass(void);

};

string KCa::getClass(){return "KCa";}


double KCa::m_inf(double V, double Ca) {return (Ca/(Ca+3))*(1.0/(1.0+exp(((V)+27.2)/-7.2)));}
double KCa::tau_m(double V, double Ca) {return 90.3-(75.1/(1.0+exp(((V)+46.0)/-22.7)));}


#endif
