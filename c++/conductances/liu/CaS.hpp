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
    CaS(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }

        supported_solver_order = 4;
    }

    void integrate(double, double);
    void integrateMS(int, double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);

};

string CaS::getClass(){
    return "CaS";
}

void CaS::integrate(double V, double Ca)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*h;

    // compute the specific calcium current and update it in the cell
    container->i_Ca += getCurrent(V);

}


// Runge-Kutta 4 integrator 
void CaS::integrateMS(int k, double V, double Ca)
{

    E = container->E_Ca;

    if (k == 0) {
        k_m[0] = dt*(mdot(V, Ca, m));
        k_h[0] = dt*(hdot(V, Ca, h));
        g = gbar*pow(m,p)*pow(h,q);
    } else if (k == 1) {
        k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
        k_h[1] = dt*(hdot(V, Ca, h + k_h[0]/2));
        g = gbar*pow(m + k_m[0]/2,p)*pow(h + k_h[0]/2,q);

    } else if (k == 2) {
        k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
        k_h[2] = dt*(hdot(V, Ca, h + k_h[1]/2));
        g = gbar*pow(m + k_m[1]/2,p)*pow(h + k_h[1]/2,q);

    } else if (k == 3) {
        k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
        k_h[3] = dt*(hdot(V, Ca, h + k_h[2]));
        g = gbar*pow(m + k_m[2],p)*pow(h + k_h[2],q);

    } else {
        // last step
        m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
        h = h + (k_h[0] + 2*k_h[1] + 2*k_h[2] + k_h[3])/6;
    }

    container->i_Ca += getCurrent(V);
}



double CaS::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+33.0)/-8.1));}
double CaS::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+60.0)/6.2));}
double CaS::tau_m(double V, double Ca) {return 1.4 + 7.0/(exp((V+27.0)/10.0) + exp((V+70.0)/-13.0));}
double CaS::tau_h(double V, double Ca) {return 60.0 + 150.0/(exp((V+55.0)/9.0) + exp((V+65.0)/-16.0));}


#endif
