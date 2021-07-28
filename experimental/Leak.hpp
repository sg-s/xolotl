// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: generic Leak conductance

#pragma once

#include "conductance.hpp"

//inherit conductance class spec
class Leak: public conductance {

public:

    // specify parameters + initial conditions
    Leak() {
        name = "Leak";
        E = -50;
    }


    Leak(double gbar_) {
        gbar = gbar_;
        name = "Leak";
        E = -50;
    }

    void integrate(double);

    

};


void Leak::integrate(double V) {
    // do nothing
    // because there is nothing to integrate
    g = gbar;
}


