// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Fast Transient Potassium Conductance
// http://www.jneurosci.org/content/21/14/5229.long
// Goldman, Golowasch, Marder, Abbott 2012
#ifndef AT
#define AT
#include "conductance.hpp"

//inherit conductance class spec
class At: public conductance {

public:


    //specify both gbar and erev and initial conditions
    At(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 0; }

        p = 1;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};

string At::getClass(){return "At";}


double At::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+23.0)/-13.0)));}
double At::tau_m(double V, double Ca) {return 11.0+(8.0/(1.0+exp(((V)+70.0)/-15.0)));}
double At::h_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+62.0)/7.0)));}
double At::tau_h(double V, double Ca) {return 6.0+(40.0/(1.0+exp(((V)-55.0)/-18.0)));}


#endif
