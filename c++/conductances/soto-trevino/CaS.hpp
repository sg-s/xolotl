// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Slow Calcium conductance
// http://jn.physiology.org/content/94/1/590.short
#ifndef CAS
#define CAS
#include "../../conductance.hpp"

//inherit conductance class spec
class CaS: public conductance {

public:

    // specify parameters + initial conditions 
    CaS(double g_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;
        
        Q_g = Q_g_;
        Q_tau_m = Q_tau_m_;
        Q_tau_h = Q_tau_h_;
    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);

};

void CaS::connect(compartment *pcomp_) {container = pcomp_; }

void CaS::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell 
    E = container->E_Ca;
    m = m_inf(V) + (m - m_inf(V))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V));
    g = pow(Q_g, delta_temp)*gbar*m*m*m;

    // compute the specific calcium current and update it in the cell 
    double this_I = g*(V-E);
    container->i_Ca += this_I;

}

double CaS::m_inf(double V) {return 1.0/(1.0+exp(-(V+22.0)/8.5));}
double CaS::tau_m(double V) {return 16 - 13.1/(1 + exp(-(V+25.1)/-26.4));}


#endif
