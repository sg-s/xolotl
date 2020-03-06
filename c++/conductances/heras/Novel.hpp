// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Potassium Conductance
// conductance first described in  (Vahasoyrinki et al. 2006)
// double shaker, shab mutants
// equations in: https://www.biorxiv.org/content/biorxiv/early/2018/06/13/344325.full.pdf
// Francisco J. H. Heras, Mikko Vahasoyrinki, and Jeremy E. Niven 2018
#ifndef NOVEL
#define NOVEL
#include "conductance.hpp"

//inherit conductance class spec
class Novel: public conductance {

public:


    //specify both gbar and erev and initial conditions
    Novel(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string Novel::getClass(){return "Novel";}


double Novel::m_inf(double V, double Ca) {return (1.0/(1.0+exp(sqrt((1.0+V)/-9.1))));}
double Novel::tau_m(double V, double Ca) {return 13.0+165.0*exp(-2.0*((V+19.4)/30.0)*((V+19.4)/30.0));}

#endif
