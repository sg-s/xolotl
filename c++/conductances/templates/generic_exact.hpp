// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// generic conductance template
// you can either fill this out yourself
// or use the conductance class within MATLAB
// to auto-generate C++ code 

#pragma once

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
        if (isnan (E)) { E = $default_E; }

        p = $p;
        q = $q;

    }

    double m_inf(double V, double Ca);
    double h_inf(double V, double Ca);
    double tau_m(double V, double Ca);
    double tau_h(double V, double Ca);
    


};


double CondName::m_inf(double V, double Ca) {return $m_inf;}
double CondName::h_inf(double V, double Ca) {return $h_inf;}
double CondName::tau_m(double V, double Ca) {return $tau_m;}
double CondName::tau_h(double V, double Ca) {return $tau_h;}

