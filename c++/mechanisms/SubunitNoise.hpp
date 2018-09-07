// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Subunit Noise generator 
// as in Goldwyn and Shea-Brown PLoS Comp Bio
// this affect a single conductance type 

#ifndef SUBUNITNOISE
#define SUBUNITNOISE
#include "mechanism.hpp"
#include <limits>
#include <random>

//inherit controller class spec
class SubunitNoise: public mechanism {

protected:
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

    void connect(conductance * cond_, synapse* syn_);
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


void SubunitNoise::connect(conductance * channel_, synapse * syn_)
{
    if (channel_)
    {
        // connect to a channel
        channel = channel_;

        controlling_class = (channel_->getClass()).c_str();
    }
    if (syn_)
    {
        // connect to a synapse 
        syn = syn_;
    }
}

void SubunitNoise::integrate(void)
{

    std::random_device generator;
    std::normal_distribution<double> distribution(0,noise_amplitude*dt);

    // double n = distribution(generator);
    // mexPrintf("n = %f\n",n);

    channel->m += distribution(generator);
    channel->h += distribution(generator);

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
