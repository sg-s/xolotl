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

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);
};

string CondName::getClass(){return "CondName";}

void CondName::integrate(double V, double Ca, double dt, double delta_temp)
{

    // clamp the voltage inside of cached range
    if (V > 101.0) {V = 101.0;}
    else if (V < -99.0) {V = -99.0;}

    
    minf = m_inf_cache[(int) round(V+99)];
    hinf = h_inf_cache[(int) round(V+99)];
    taum = tau_m_cache[(int) round(V+99)];
    tauh = tau_h_cache[(int) round(V+99)];

    m = minf + (m - minf)*exp(-(dt/taum));
    h = hinf + (h - hinf)*exp(-(dt/tauh));
    $GBAR=?
}

double CondName::m_inf(double V) {return $m_inf;}
double CondName::h_inf(double V) {return $h_inf;}
double CondName::tau_m(double V) {return $tau_m;}
double CondName::tau_h(double V) {return $tau_h;}

#endif
