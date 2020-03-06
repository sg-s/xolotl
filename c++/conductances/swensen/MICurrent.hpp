// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Modulatory Input Conductance
// http://www.jneurosci.org.resources.library.brandeis.edu/content/jneuro/21/11/4050.full.pdf
// based on RPCH/proctolin recordings in Swensen & Marder 2001
#ifndef MICURRENT
#define MICURRENT
#include "conductance.hpp"

//inherit conductance class spec
class MICurrent: public conductance {

public:


    //specify both gbar and erev and initial conditions
    MICurrent(double gbar_, double E_, double m_)
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

    double m_inf(double V, double Ca);
    double h_inf(double V, double Ca);
    double tau_m(double V, double Ca);
    double tau_h(double V, double Ca);
    string getClass(void);

};

string MICurrent::getClass(){return "MICurrent";}

double MICurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+21.0)/-8));}
double MICurrent::tau_m(double V, double Ca) {return 6.0;}

double MICurrent::h_inf(double V, double Ca) {return   0;}
double MICurrent::tau_h(double V, double Ca) {return   0;}

#endif
