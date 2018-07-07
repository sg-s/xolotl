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
    Novel(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};

string Novel::getClass(){return "Novel";}

void Novel::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m;
}


double Novel::m_inf(double V) {return (1.0/(1.0+exp((-1.0-V)/9.1)))^(1.0/2.0);}
double Novel::tau_m(double V) {return 13.0+165.0*exp(-2.0*((V+19.4)/30.0)^2.0);}

#endif
