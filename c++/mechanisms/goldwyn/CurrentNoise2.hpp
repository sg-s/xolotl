// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Current Noise generator
// component source: [Goldwyn and Shea-Brown](https://journals.plos.org/ploscompbiol/article?id=10.1371/journal.pcbi.1002247)
// 
// uses a faster RNG

#ifndef CURRENTNOISE22
#define CURRENTNOISE22
#include "mechanism.hpp"
#include <limits>
#include <random>

//inherit controller class spec
class CurrentNoise2: public mechanism {

protected:


public:


    // scale
    double noise_amplitude = 0;


    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CurrentNoise2(double noise_amplitude_)
    {
        noise_amplitude = noise_amplitude_;
        controlling_class = "unset";

    }


    double gaussrand(void);

    void integrate(void);
    void checkSolvers(int);

    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);


    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

};

string CurrentNoise2::getClass() {
    return "CurrentNoise2";
}

double CurrentNoise2::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();
}


int CurrentNoise2::getFullStateSize() {return 0; }


int CurrentNoise2::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}


void CurrentNoise2::connect(compartment* comp_)
{
    comp = comp_;
    comp->addMechanism(this);
}

void CurrentNoise2::connect(conductance* cond_)
{
    mexErrMsgTxt("[CurrentNoise2] This mechanism cannot connect to a conductance object");
}

void CurrentNoise2::connect(synapse* syn_)
{
    mexErrMsgTxt("[CurrentNoise2] This mechanism cannot connect to a synapse object");
}


void CurrentNoise2::integrate(void)
{
    comp->I_ext += gaussrand()*noise_amplitude*dt;
}

void CurrentNoise2::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[CurrentNoise2] unsupported solver order\n");
    }
}

// originally from Knuth and Marsaglia
// see "A Convenient Method for Generating Normal Variables"
// SIAM Rev., 6(3), 260–264.
double CurrentNoise2::gaussrand()
{
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if(phase == 0) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
            } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return X;
}


#endif
