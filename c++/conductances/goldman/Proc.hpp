// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Dynamic-Clamp-Added Proctolin-Activated Conductance
// http://www.jneurosci.org/content/21/14/5229.long
// Goldman, Golowasch, Marder, Abbott 2012
#ifndef PROC
#define PROC


//inherit conductance class spec
class Proc: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Proc(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = 0; }

        p = 1;

        name = "Proc";

    }

    double m_inf(double, double);
    double tau_m(double, double);
    

};




double Proc::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+57.0)/-5.0)));}
double Proc::tau_m(double V, double Ca) {return 6.0;}


#endif
