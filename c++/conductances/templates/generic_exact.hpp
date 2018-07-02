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
    CondName(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = $default_m; }
        if (isnan (h)) { h = $default_h; }
        if (isnan (E)) { E = $default_E; }

    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V, double Ca);
    double h_inf(double V, double Ca);
    double tau_m(double V, double Ca);
    double tau_h(double V, double Ca);
    string getClass(void);


};

string CondName::getClass(){return "CondName";}

void CondName::integrate(double V, double Ca, double dt, double delta_temp)
{
    m = m_inf(V, Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V, Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    $GBAR=?

}

double CondName::m_inf(double V, double Ca) {return $m_inf;}
double CondName::h_inf(double V, double Ca) {return $h_inf;}
double CondName::tau_m(double V, double Ca) {return $tau_m;}
double CondName::tau_h(double V, double Ca) {return $tau_h;}

#endif
