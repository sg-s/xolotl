// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Current Noise generator
// component source  [Goldwyn and Shea-Brown](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1002247)
// 
//

#pragma once
#include "mechanism.hpp"
#include <limits>
#include <random>

//inherit controller class spec
class CurrentNoise: public mechanism {

protected:

    std::random_device generator;
    std::normal_distribution<double> distribution;


public:


    // scale
    double noise_amplitude = 0;


    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CurrentNoise(double noise_amplitude_)
    {
        noise_amplitude = noise_amplitude_;
        controlling_class = "unset";
        name = "CurrentNoise";

    }

    void integrate(void);

    void connectCompartment(compartment*);


    

};


void CurrentNoise::connectCompartment(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}



void CurrentNoise::integrate(void) {
    comp->I_ext += distribution(generator)*noise_amplitude*dt;
}



