// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Persisent Sodium
// http://jn.physiology.org/content/94/1/590.short

#ifndef PROC
#define PROC
#include "conductance.hpp"

//inherit conductance class spec
class Proc: public conductance {

public:

    //specify both gbar and erev and initial conditions
    Proc(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = 0; }
    }


    double m_inf(double, double);
    string getClass(void);

};

string Proc::getClass(){return "Proc";}

double Proc::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+12.0)/3.05));}



#endif
