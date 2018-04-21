// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Modulatory Input Conductance
// https://www.ncbi.nlm.nih.gov/pubmed/8463821
// based on dynamic clamp in Sharp et al 1993, Bucholtz et al 1993, Golowasch et al 1992
#ifndef MICURRENT
#define MICURRENT
#include "../../conductance.hpp"

//inherit conductance class spec
class MICurrent: public conductance {

public:


    //specify both gbar and erev and initial conditions
    MICurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;
    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);

};

string MICurrent::getClass(){return "MICurrent";}

void MICurrent::connect(compartment *pcomp_) {container = pcomp_;}

void MICurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m;
}


double MICurrent::m_inf(double V) {return 1.0/(1.0+exp((V+55.0)/-5));}
double MICurrent::tau_m(double V) {return 6.0;}


#endif
