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
    Proc(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

         // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 0; }
    }


    void integrate(double, double);

    double m_inf(double, double);
    string getClass(void);

};

string Proc::getClass(){return "Proc";}

void Proc::integrate(double V, double Ca)
{
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/0.5);
    g = gbar*m;
}


double Proc::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+12.0)/3.05));}



#endif
