// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Cac channels in Drosophila muscles
// https://www.ncbi.nlm.nih.gov/pubmed/26004544
#ifndef CAC
#define CAC
#include "../../conductance.hpp"

//inherit conductance class spec
class Cac: public conductance {

public:

    // specify parameters + initial conditions 
    Cac(double g_, double E_, double m_, double h_)
{
    gbar = g_;
    E = E_;
    m = m_;
    h = h_;

    // defaults
    if (isnan (m)) { m = 0; }
    if (isnan (h)) { h = 1; }
    if (isnan (E)) { E = 30; }

}

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V); 
    string getClass(void);
};

string Cac::getClass(){return "Cac";}

void Cac::connect(compartment *pcomp_) {container = pcomp_; }

void Cac::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell 
    E = container->E_Ca;
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*h;

    // compute the specific calcium current and update it in the cell 
    double this_I = g*(V-E);
    container->i_Ca += this_I;

}

double Cac::m_inf(double V) {return 1.0/(1.0+exp((V-3.68)/-6.35));}
double Cac::h_inf(double V) {return 1.0/(1.0+exp((V+9.17)/8.22));}
double Cac::tau_m(double V) {return 2.8 + 14.0/(exp((V+27.0)/10.0) + exp((V+70.0)/-13.0));}
double Cac::tau_h(double V) {return 1400;}

#endif
