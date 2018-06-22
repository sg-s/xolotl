// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Deleyed rectifier
// Dethier, Drion, Franci, Sepulchre 2015
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4620135/

#ifndef CAT
#define CAT
#include "conductance.hpp"

//inherit conductance class spec
class CaT: public conductance {

public:

    // specify parameters + initial conditions
    CaT(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 0; }
        if (isnan (E)) { E = 30; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double tau_m(double V);
    double h_inf(double V);
    double tau_h(double V);
    string getClass(void);


};

string CaT::getClass(){return "CaT";}

void CaT::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-dt/tau_h(V));
    g = gbar*m*m*m*h;
}

double CaT::m_inf(double V) {return (1.0/(1.0+exp(((V)+57.1)/-7.2)));}
double CaT::h_inf(double V) {return (1.0/(1.0+exp(((V)+82.1)/5.5)));}
double CaT::tau_m(double V) {return 43.4- (42.6/(1.0+exp(((V)+68.1)/-20.5)));}
double CaT::tau_h(double V) {return 840.0- (718.4/(1.0+exp(((V)+55.0)/-16.9)));}

#endif
