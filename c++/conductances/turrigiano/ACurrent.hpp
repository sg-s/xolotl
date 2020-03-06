// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// the A current, a potassium current
// for some reason I don't understand, I get compiler
// errors when I name this "A" or "Ka"
// so we'll have to live with this awkward name
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef ACURRENT
#define ACURRENT
#include "conductance.hpp"

//inherit conductance class spec
class ACurrent: public conductance {

public:

    // specify parameters + initial conditions
    ACurrent(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = -80; }
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};


string ACurrent::getClass(){
    return "ACurrent";
}

double ACurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+27.2)/-8.7)); }
double ACurrent::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+56.9)/4.9)); }
double ACurrent::tau_m(double V, double Ca) {return 11.6 - 10.4/(1.0+exp((V+32.9)/-15.2));}
double ACurrent::tau_h(double V, double Ca) {return 38.6 - 29.2/(1.0+exp((V+38.9)/-26.5));}


#endif
