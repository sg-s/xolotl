// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Fast sodium conductance 
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)

#pragma once

#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {


public:

    NaV() {
        p = 3;
        q = 1;
        name = "NaV";
        E = 50;
    }

    NaV(double gbar_) {
        gbar = gbar_;
        p = 3;
        E = 50;
        q = 1;
        name = "NaV";
    }

    double m_inf(double) override;
    double h_inf(double) override;
    double tau_m(double) override;
    double tau_h(double) override;
};



double NaV::m_inf(double V) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double NaV::h_inf(double V) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double NaV::tau_m(double V) {return 1.32 - 1.26/(1+exp((V+120.0)/-25.0));}
double NaV::tau_h(double V) {return (0.67/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6)));}

