// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Fast Calcium CONDUCTANCE
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
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
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }
    
        // cache values for m_inf and h_inf
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V);
            h_inf_cache[(int) round(V+99)] = h_inf(V);
            tau_m_cache[(int) round(V+99)] = tau_m(V);
            tau_h_cache[(int) round(V+99)] = tau_h(V);
        }

    }

    double m_inf_cache[200];
    double h_inf_cache[200];
    double tau_m_cache[200];
    double tau_h_cache[200];

    double taum;
    double tauh;
    double minf;
    double hinf;

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void); 
};

string CaT::getClass(){return "CaT";}

void CaT::connect(compartment *pcomp_) { container = pcomp_; }

void CaT::integrate(double V, double Ca, double dt, double delta_temp)
{
    // update E by copying E_Ca from the cell 
    E = container->E_Ca;
    
    minf = m_inf_cache[(int) round(V+99)];
    hinf = h_inf_cache[(int) round(V+99)];
    taum = tau_m_cache[(int) round(V+99)];
    tauh = tau_h_cache[(int) round(V+99)];

    m = minf + (m - minf)*exp(-(dt/taum));
    h = hinf + (h - hinf)*exp(-(dt/tauh));
    
    g = gbar*m*m*m*h;

    // compute the specific calcium current and update it in the cell 
    double this_I = g*(V-E);
    container->i_Ca += this_I;
}


double CaT::m_inf(double V) {return 1.0/(1.0 + exp((V+27.1)/-7.2));}
double CaT::h_inf(double V) {return 1.0/(1.0 + exp((V+32.1)/5.5));}
double CaT::tau_m(double V) {return 21.7 - 21.3/(1.0 + exp((V+68.1)/-20.5));}
double CaT::tau_h(double V) {return 105.0 - 89.8/(1.0 + exp((V+55.0)/-16.9));}

#endif
