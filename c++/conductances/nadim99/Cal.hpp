// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// calcium conductance
// Nadim, Manor, Kopell, Marder 1999
// Hyperlink:http://www.pnas.org/content/96/14/8206.full

#ifndef CAL
#define CAL
#include "conductance.hpp"

//inherit conductance class spec
class Cal: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Cal(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 1;
        q = 1;

        is_calcium = true;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (h)) { h = 0; }
        if (isnan (E)) { E = -80; }

        name = "Cal";
    }


    double m_inf(double, double);
    double h_inf(double, double);
    double tau_h(double, double);
    

};



double Cal::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+61.0)/-4.2)));}
double Cal::h_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+88.0)/8.5)));}
double Cal::tau_h(double V, double Ca) {return 350.0;}

#endif
