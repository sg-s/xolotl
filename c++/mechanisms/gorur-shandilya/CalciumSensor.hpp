// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: sensor of intracellular calcium
// component source: [Gorur-Shandilya et al. 2019](https://www.biorxiv.org/content/10.1101/753608v1.abstract)

#pragma once
#include <limits>

//inherit mechanism class spec
class CalciumSensor: public mechanism {

protected:
public:


    // scale
    double tau = 5e3;

    double Ca_average;


    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    CalciumSensor(double tau_, double Ca_average_) {
        tau = tau_;
        controlling_class = "unset";
        Ca_average = Ca_average_;

        fullStateSize = 1;

        name = "CalciumSensor";
    }


    void integrate(void);
    void init(void);


    int getFullState(double * cont_state, int idx);
    

};


void CalciumSensor::init(void) {
    if (isnan(Ca_average)) {
        Ca_average = comp->Ca;
    }
}


int CalciumSensor::getFullState(double *cont_state, int idx) {
    // return internal variable
    cont_state[idx] = Ca_average;
    idx++;
    return idx;
}


void CalciumSensor::integrate(void) {
    Ca_average += (dt/tau)*(comp->Ca - Ca_average);
}



