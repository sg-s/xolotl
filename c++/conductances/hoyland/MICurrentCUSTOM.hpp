// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Modulatory Input Conductance
// http://www.physiology.org/doi/abs/10.1152/jn.1993.69.3.992?url_ver=Z39.88-2003&rfr_id=ori:rid:crossref.org&rfr_dat=cr_pub%3dpubmed
// based on RPCH/proctolin recordings
#ifndef MICURRENTUSTOM
#define MICURRENTCUSTOM
#include "../../conductance.hpp"

//inherit conductance class spec
class MICurrentCUSTOM: public conductance {

public:


    //specify both gbar and erev and initial conditions
    MICurrentCUSTOM(double g_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = 1;


        Vth = Q_g_;
        Vw = Q_tau_m_;
        tc = Q_tau_h_;
    }
    
    double Vth;
    double Vw;
    double tc;

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);

};

void MICurrentCUSTOM::connect(compartment *pcomp_) {container = pcomp_;}

void MICurrentCUSTOM::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))/tau_m(V);
    g = gbar*m;
}


double MICurrentCUSTOM::m_inf(double V) {return 1.0/(1.0+exp((V-Vth)/-Vw));}
double MICurrentCUSTOM::tau_m(double V) {return tc;}


#endif
