// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// generic conductance template
// you can either fill this out yourself
// or use the conductance class within MATLAB
// to auto-generate C++ code 

#ifndef UPPERCASENAME
#define UPPERCASENAME
#include "conductance.hpp"

//inherit conductance class spec
class CondName: public conductance {

public:

    // specify parameters + initial conditions
    CondName(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = $default_m; }
        if (isnan (h)) { h = $default_h; }
        if (isnan (E)) { E = $default_E; }

        p = $p;
        q = $q;
    }

    void integrate(double, double);
    void integrateMS(int, double, double);

    double m_inf(double V, double Ca);
    double h_inf(double V, double Ca);
    double tau_m(double V, double Ca);
    double tau_h(double V, double Ca);
    string getClass(void);


};

string CondName::getClass(){return "CondName";}

void CondName::integrate(double V, double Ca) {
    E = container->E_Ca;
    conductance::integrate(V,Ca);
    container->i_Ca += getCurrent(V);
}

void CondName::integrateMS(int k, double V, double Ca) {
    E = container->E_Ca;
    conductance::integrateMS(k, V, Ca);
    container->i_Ca += getCurrent(V);
}

double CondName::m_inf(double V, double Ca) {return $m_inf;}
double CondName::h_inf(double V, double Ca) {return $h_inf;}
double CondName::tau_m(double V, double Ca) {return $tau_m;}
double CondName::tau_h(double V, double Ca) {return $tau_h;}


#endif
