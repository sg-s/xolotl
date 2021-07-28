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
    Leak()
    {
        name = "Leak";
    }


    void integrate(double, double);

    

};


void Leak::integrate(double V, double Ca) {
    // do nothing
    // because there is nothing to integrate
    g = gbar;
}


