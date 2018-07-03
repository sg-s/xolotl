// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Current Noise generator 
// as in Goldwyn and Shea-Brown PLoS Comp Bio
// 

#ifndef CURRENTNOISE
#define CURRENTNOISE
#include "mechanism.hpp"
#include <limits>
#include <random>

//inherit controller class spec
class CurrentNoise: public mechanism {

protected:
public:


    // scale
    double noise_amplitude; 


    // specify parameters + initial conditions for 
    // mechanism that controls a conductance 
    CurrentNoise(double noise_amplitude_)
    {
        noise_amplitude = noise_amplitude_;
        if (isnan (noise_amplitude)) { noise_amplitude = 0; }
        controlling_class = "unset";


        

    }

    
    void integrate(double dt);
    void connect(compartment * comp_);
    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

};


double CurrentNoise::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();

}


int CurrentNoise::getFullStateSize()
{
    return 0; 
}


int CurrentNoise::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}


void CurrentNoise::connect(compartment* comp_)
{
    comp = comp_;
}

void CurrentNoise::integrate(double dt)
{

    std::random_device generator;
    std::normal_distribution<double> distribution(0,noise_amplitude*dt);

    // double n = distribution(generator);
    // mexPrintf("n = %f\n",n);

    comp->I_ext += distribution(generator);


}



#endif
