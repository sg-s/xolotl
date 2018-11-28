// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Subunit Noise generator
// as in Goldwyn and Shea-Brown PLoS Comp Bio
// https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1002247
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

    }


    void integrate(void);
    void checkSolvers(int);

    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

};


double SubunitNoise::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();

}


int SubunitNoise::getFullStateSize()
{
    return 0;
}


int SubunitNoise::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}


void SubunitNoise::connect(conductance * channel_)
{

    // connect to a channel
    channel = channel_;
    (channel->container)->addMechanism(this);
    controlling_class = (channel_->getClass()).c_str();


}

void SubunitNoise::connect(compartment* comp_)
{
    mexErrMsgTxt("[SubunitNoise] This mechanism cannot connect to a compartment object");
}

void SubunitNoise::connect(synapse* syn_)
{
    mexErrMsgTxt("[SubunitNoise] This mechanism cannot connect to a synapse object");
}

void SubunitNoise::integrate(void)
{

    channel->m += distribution(generator)*noise_amplitude*dt;;
    channel->h += distribution(generator)*noise_amplitude*dt;;

    if ((channel->m) < 0) {channel->m = 0;}
    if ((channel->h) < 0) {channel->h = 0;}
    if ((channel->m) > 1) {channel->m = 1;}
    if ((channel->h) > 1) {channel->h = 1;}

}

void SubunitNoise::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[SubunitNoise] unsupported solver order\n");
    }
}



#endif
