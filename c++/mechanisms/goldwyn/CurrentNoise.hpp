// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Current Noise generator
// as in Goldwyn and Shea-Brown PLoS Comp Bio
// https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1002247
//

#ifndef CURRENTNOISE
#define CURRENTNOISE
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


double CurrentNoise::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();
}


int CurrentNoise::getFullStateSize() {return 0; }


int CurrentNoise::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}


void CurrentNoise::connect(compartment* comp_)
{
    comp = comp_;
    comp->addMechanism(this);
}

void CurrentNoise::connect(conductance* cond_)
{
    mexErrMsgTxt("[CurrentNoise] This mechanism cannot connect to a conductance object");
}

void CurrentNoise::connect(synapse* syn_)
{
    mexErrMsgTxt("[CurrentNoise] This mechanism cannot connect to a synapse object");
}


void CurrentNoise::integrate(void)
{
    comp->I_ext += distribution(generator)*noise_amplitude*dt;
}

void CurrentNoise::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[CurrentNoise] unsupported solver order\n");
    }
}



#endif
