// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// T-type calcium conductance
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879

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
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }

        p = 2;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

    }

    void integrate(double, double);
    void integrateMS(int, double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string CaT::getClass(){
    return "CaT";
}

void CaT::integrate(double V, double Ca) {
    E = container->E_Ca;
    m = m_inf(V, Ca);
    h = h_inf(V, Ca) + (h - h_inf(V, Ca))*exp(-dt/tau_h(V, Ca));
    g = gbar * m * m * h;
    container->i_Ca += getCurrent(V);
}

void CaT::integrateMS(int k, double V, double Ca)
{
    E = container->E_Ca;

    switch (k)
    {
        case 0:
            k_m[0] = dt*m_inf(V, Ca);
            k_h[0] = dt*(hdot(V, Ca, h));
            g = gbar*fast_pow(m,p)*fast_pow(h,q);
            break;
        case 1:
            k_m[1] = dt*m_inf(V, Ca);
            k_h[1] = dt*(hdot(V, Ca, h + k_h[0]/2));
            g = gbar*fast_pow(m + k_m[0]/2,p)*fast_pow(h + k_h[0]/2,q);
            break;
        case 2:
            k_m[2] = dt*m_inf(V, Ca);
            k_h[2] = dt*(hdot(V, Ca, h + k_h[1]/2));
            g = gbar*fast_pow(m + k_m[1]/2,p)*fast_pow(h + k_h[1]/2,q);
            break;
        case 3:
            k_m[3] = dt*m_inf(V, Ca);
            k_h[3] = dt*(hdot(V, Ca, h + k_h[2]));
            g = gbar*fast_pow(m + k_m[2],p)*fast_pow(h + k_h[2],q);
            break;
        case 4:
            m = m_inf(V, Ca);
            h = h + (k_h[0] + 2*k_h[1] + 2*k_h[2] + k_h[3])/6;
            break;
    }

    gbar = gbar_next;

    container->i_Ca += getCurrent(V);
}

double CaT::m_inf(double V, double Ca) {return 1.0/(1.0+exp((-((V+4.0)+57.0))/6.2)); }
double CaT::h_inf(double V, double Ca) {return 1.0/(1.0+exp(((V+4.0)+81.0)/4.0)); }
double CaT::tau_m(double V, double Ca) {return 0.0; }
double CaT::tau_h(double V, double Ca) {return (30.8+(211.4+exp(((V+4.0)+113.2)/5.0))/(1.0+exp(((V+2.0)+84.0)/3.2)))/3.73; }


#endif
