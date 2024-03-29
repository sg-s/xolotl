// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Ca-activated potassium conductance 
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//
#pragma once
class conductance;

//inherit conductance class spec
class KCa: public conductance {

public:

    // specify parameters + initial conditions
    KCa(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults 
        if (isnan (E)) { E = -80; }

        p = 4;

        AllowMInfApproximation = false;
        AllowHInfApproximation = false;

        name = "KCa";

    }

    double m_inf(double V, double Ca);
    double tau_m(double, double);


};


double KCa::m_inf(double V, double Ca) { return (Ca/(Ca+3.0))/(1.0+exp((V+28.3)/-12.6)); }
double KCa::tau_m(double V, double Ca) {return 90.3 - 75.1/(1.0+exp((V+46.0)/-22.7));}



