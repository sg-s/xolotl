// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Fast Calcium CONDUCTANCE
// this version does not support temperature dependence
// http://jn.physiology.org/content/jn/90/6/3998.full.pdf
#ifndef CAL
#define CAL
#include "conductance.hpp"

//inherit conductance class spec
class Cal: public conductance {

public:

    // specify parameters + initial conditions
    Cal(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
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

    double m_alpha(double, double);
    double m_beta(double, double);
    double h_alpha(double, double);
    double h_beta(double, double);

    string getClass(void);


};

string Cal::getClass(){return "Cal";}

void Cal::integrate(double V, double Ca) {
    E = container->E_Ca;
    conductance::integrate(V,Ca);
    container->i_Ca += getCurrent(V);
}

void Cal::integrateMS(int k, double V, double Ca) {
    E = container->E_Ca;
    conductance::integrateMS(k, V, Ca);
    container->i_Ca += getCurrent(V);
}

double Cal::m_alpha(double V, double Ca) {return 1.6 / (1.0 + exp(-0.072 * (V - 65.0)));}
double Cal::m_beta(double V, double Ca) {return 0.02 * (V - 51.1) / (exp((V-51.1)/5.0) - 1.0);}

double Cal::h_alpha(double V, double Ca)
{
    if (V > 0)
    {
        return exp(-V/20.0)/200.0;
    } else {
        return 0.005;
    }
}

double Cal::h_beta(double V, double Ca)
{
    if (V > 0)
    {
        return 0.005 - h_alpha(V, Ca);
    } else {
        return 0;
    }
}

double Cal::m_inf(double V, double Ca) {return m_alpha(V, Ca) / (m_alpha(V, Ca) + m_beta(V, Ca));}
double Cal::h_inf(double V, double Ca) {return h_alpha(V, Ca) / (h_alpha(V, Ca) + h_beta(V, Ca));}
double Cal::tau_m(double V, double Ca) {return 1.0 / (m_alpha(V, Ca) + m_beta(V, Ca));}
double Cal::tau_h(double V, double Ca) {return 1.0 / (h_alpha(V, Ca) + h_beta(V, Ca));}


#endif
