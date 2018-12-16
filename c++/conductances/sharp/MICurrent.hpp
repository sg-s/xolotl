// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Modulatory Input Conductance
// https://www.ncbi.nlm.nih.gov/pubmed/8463821
// based on dynamic clamp in Sharp et al 1993, Bucholtz et al 1993, Golowasch et al 1992
#ifndef MICURRENT
#define MICURRENT
#include "conductance.hpp"

//inherit conductance class spec
class MICurrent: public conductance {

public:


    //specify both gbar and erev and initial conditions
    MICurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;


         // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -20; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string MICurrent::getClass(){return "MICurrent";}

double MICurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+55.0)/-5));}
double MICurrent::tau_m(double V, double Ca) {return 6.0;}


#endif
