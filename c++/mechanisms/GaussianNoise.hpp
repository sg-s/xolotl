// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component source [Gorur-Shandilya et al.](https://www.frontiersin.org/articles/10.3389/fninf.2018.00087/full)
// component info: a gaussian noise generator that can be connected to other mechanisms 

#ifndef GAUSSIANNOISE
#define GAUSSIANNOISE
#include <limits>

//inherit controller class spec
class GaussianNoise: public mechanism {

protected:


public:


    double noise = 0;
    double noise_amplitude = 0;


    // how many dimensions should the GaussianNoise have?
    int dimension = 1;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    GaussianNoise(double noise_amplitude_, double dimension_) {

        noise_amplitude = noise_amplitude_;
        if (isnan(noise_amplitude)) {noise_amplitude = 0;}

        if (isnan(dimension_)) {dimension_ = 1;}

        dimension = (int) dimension;

        name = "GaussianNoise";
        mechanism_type = "AdditiveNoise";

        fullStateSize = dimension;
    }



    // method declarations
    void integrate(void);
    double getState(int);
    

};

// doesn't matter what the index is, always return the same noise
double GaussianNoise::getState(int i) {
    return noise;
}



void GaussianNoise::integrate(void) {
    noise = noise_amplitude*(conductance::gaussrand());
}



#endif
