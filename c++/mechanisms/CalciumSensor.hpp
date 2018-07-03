// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Calcium sensor
// this mechanism averages the Calcium
// in the compartment it is in 
// over some time window
// useful to measure Calcium, and
// also useful in other mechanisms

#ifndef CALCIUMSENSOR
#define CALCIUMSENSOR
#include "mechanism.hpp"
#include <limits>

//inherit mechanism class spec
class CalciumSensor: public mechanism {

protected:
public:


    // scale
    double tau; 

    double Ca_average;


    // specify parameters + initial conditions for 
    // mechanism that controls a conductance 
    CalciumSensor(double tau_)
    {
        tau = tau_;
        if (isnan (tau)) { tau = 5e3; }
        controlling_class = "unset";

    }

    
    void integrate(double dt);
    void connect(compartment* );
    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

};


double CalciumSensor::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();

}


int CalciumSensor::getFullStateSize()
{
    return 1; 
}


int CalciumSensor::getFullState(double *cont_state, int idx)
{
    // return internal variable
    cont_state[idx] = Ca_average;
    idx++;
    return idx;
}


void CalciumSensor::connect(compartment* comp_)
{
    comp = comp_;
    Ca_average = comp->Ca;
}

void CalciumSensor::integrate(double dt)
{
    Ca_average += (dt/tau)*(comp->Ca - Ca_average);
}



#endif
