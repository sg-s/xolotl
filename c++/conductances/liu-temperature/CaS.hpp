// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow Calcium conductance
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef CAS
#define CAS
#include "conductance.hpp"

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

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (Q_g)) { Q_g = 1; }
        if (isnan (Q_tau_m)) { Q_tau_m = 1; }
        if (isnan (Q_tau_h)) { Q_tau_h = 1; }
        if (isnan (E)) { E = 30; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);
    double getCurrent(double V, double Ca);

};

string CaS::getClass(){return "CaS";}

void CaS::connect(compartment *pcomp_) {container = pcomp_; }

void CaS::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;
    m = m_inf(V) + (m - m_inf(V))*exp(-(dt*pow(Q_tau_m, delta_temp))/tau_m(V));
    h = h_inf(V) + (h - h_inf(V))*exp(-(dt*pow(Q_tau_h, delta_temp))/tau_h(V));
    g = pow(Q_g, delta_temp)*gbar*m*m*m*h;

    // compute the specific calcium current and update it in the cell
    double this_I = g*(V-E);
    container->i_Ca += this_I;

}

double CaS::m_inf(double V) {return 1.0/(1.0+exp((V+33.0)/-8.1));}
double CaS::h_inf(double V) {return 1.0/(1.0+exp((V+60.0)/6.2));}
double CaS::tau_m(double V) {return 1.4 + 7.0/(exp((V+27.0)/10.0) + exp((V+70.0)/-13.0));}
double CaS::tau_h(double V) {return 60.0 + 150.0/(exp((V+55.0)/9.0) + exp((V+65.0)/-16.0));}

double CaS::getCurrent(double V, double Ca) {return gbar*m*m*m*(V-E);}

#endif
