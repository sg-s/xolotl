// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Inactivating Calcium conductance
// from Turrigiano ... Marder 1995
// http://www.jneurosci.org/content/jneuro/15/5/3640.full.pdf

#ifndef CA2
#define CA2
#include "../../conductance.hpp"

//inherit conductance class spec
class Ca2: public conductance {

public:

    // specify parameters + initial conditions 
    Ca2(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 120; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);
};

string Ca2::getClass(){
    return "Ca2";
}

void Ca2::connect(compartment *pcomp_) {container = pcomp_; }

void Ca2::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell 
    E = container->E_Ca;
    m = m_inf(V) + (m - m_inf(V))*exp(-dt/tau_m(V));
    g = gbar*m*m*m;

    // compute the specific calcium current and update it in the cell 
    double this_I = g*(V-E);
    container->i_Ca += this_I;

}

double Ca2::m_inf(double V) {return 1.0/(1.0+exp((V+21.6)/-8.5));}
double Ca2::tau_m(double V) {return 16 - 13.1/(exp((V+25.1)/-26.4));}

#endif
