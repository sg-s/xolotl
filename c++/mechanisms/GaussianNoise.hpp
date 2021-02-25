// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component source [Gorur-Shandilya et al.](https://www.frontiersin.org/articles/10.3389/fninf.2018.00087/full)
// component info: a gaussian noise generator that can be connected to other mechanisms 

#ifndef GAUSSIANNOISE
#define GAUSSIANNOISE
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class GaussianNoise: public mechanism {

protected:


public:


    double noise = 0;
    double noise_amplitude = 0;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    GaussianNoise(double noise_amplitude_) {

        noise_amplitude = noise_amplitude_;
        if (isnan(noise_amplitude)) {noise_amplitude = 0;}

        name = "GaussianNoise";
        mechanism_type = "NoiseTerm";

        fullStateSize = 1;
    }



    // method declarations
    void integrate(void);
    void connectConductance(conductance*);
    void connectCompartment(compartment*);
    double getState(int);
    

};


double GaussianNoise::getState(int i) {
    return noise;
}


void GaussianNoise::connectCompartment(compartment * comp_) {
    comp = comp_;
    comp->addMechanism(this);
}


void GaussianNoise::connectConductance(conductance * channel_) {

    // connect to a channel
    channel = channel_;
    comp = channel->container;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

    controlling_class = (channel_->name).c_str();


}


void GaussianNoise::integrate(void) {
    noise = dt*noise_amplitude*(conductance::gaussrand());
}



#endif
