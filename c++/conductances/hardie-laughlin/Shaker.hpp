// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Shaker current
// an inactivating K current in Drosophila
// first characterized here: http://www.sciencedirect.com/science/article/pii/089662739190255X

#ifndef SHAKER
#define SHAKER
#include "../../conductance.hpp"

//inherit conductance class spec
class Shaker: public conductance {

public:

    // specify parameters + initial conditions 
    Shaker(double g_, double E_, double m_, double h_, double Q_g_, double Q_tau_m_, double Q_tau_h_)
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
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V); 
};

void Shaker::connect(compartment *pcomp_) {container = pcomp_;}

void Shaker::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V) + (m - m_inf(V))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-(dt*pow(Q_tau_h, delta_temp))/tau_h(V));
    g = pow(Q_g, delta_temp)*gbar*m*h;
}

double Shaker::m_inf(double V) {return 1.0/(1.0+exp((V+62.8)/-8.2)); }
double Shaker::h_inf(double V) {return 1.0/(1.0+exp((V+80.0)/5.23)); }
double Shaker::tau_m(double V) {return 50.0 - 47.88/(1.0+exp((V+92.24)/-8.0));}
double Shaker::tau_h(double V) {return 50;}


#endif
