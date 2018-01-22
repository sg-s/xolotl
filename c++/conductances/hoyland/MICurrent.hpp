// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Modulatory Input Conductance
// http://www.physiology.org/doi/abs/10.1152/jn.1993.69.3.992?url_ver=Z39.88-2003&rfr_id=ori:rid:crossref.org&rfr_dat=cr_pub%3dpubmed
// based on RPCH/proctolin recordings
#ifndef MICURRENT
#define MICURRENT
#include "../../conductance.hpp"

//inherit conductance class spec
class MICurrent: public conductance {

public:


    //specify both gbar and erev and initial conditions
    MICurrent(double g_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;


        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;
        Q_tau_h = Q_tau_h_;
    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);

};

void MICurrent::connect(compartment *pcomp_) {container = pcomp_;}

void MICurrent::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V));
    g = pow(Q_g, delta_temp)*gbar*m;
}


double MICurrent::m_inf(double V) {return 1.0/(1.0+exp((V+55.0)/-5));}
double MICurrent::tau_m(double V) {return 6.0;}


#endif
