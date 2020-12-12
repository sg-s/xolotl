// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// 
// component info: Subunit Noise generator
// component source: [Goldwyn and Shea-Brown](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1002247)
// this affect a single conductance type

#ifndef SUBUNITNOISE
#define SUBUNITNOISE
#include "mechanism.hpp"
#include <limits>
#include <random>

//inherit controller class spec
class SubunitNoise: public mechanism {

protected:
    std::random_device generator;
    std::normal_distribution<double> distribution;
public:


    // scale
    double noise_amplitude = 0;


    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    SubunitNoise(double noise_amplitude_)
    {
        noise_amplitude = noise_amplitude_;
        controlling_class = "unset";

        name = "SubunitNoise";

    }


    void integrate(void);


    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);


    

};



void SubunitNoise::connect(conductance * channel_) {

    // connect to a channel
    channel = channel_;
    (channel->container)->addMechanism(this);
    controlling_class = (channel_->name).c_str();

}

void SubunitNoise::integrate(void) {

    channel->m += distribution(generator)*noise_amplitude*dt;;
    channel->h += distribution(generator)*noise_amplitude*dt;;

    if ((channel->m) < 0) {channel->m = 0;}
    if ((channel->h) < 0) {channel->h = 0;}
    if ((channel->m) > 1) {channel->m = 1;}
    if ((channel->h) > 1) {channel->h = 1;}

}


#endif
