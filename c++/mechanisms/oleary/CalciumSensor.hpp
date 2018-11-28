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
    double tau = 5e3; 

    double Ca_average;


    // specify parameters + initial conditions for 
    // mechanism that controls a conductance 
    CalciumSensor(double tau_, double Ca_average_)
    {
        tau = tau_;
        controlling_class = "unset";
        Ca_average = Ca_average_;

    }

    
    void integrate(void);


    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    void checkSolvers(int);

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
    if (isnan(Ca_average)) {Ca_average = comp->Ca;}
    comp->addMechanism(this);
}

void CalciumSensor::connect(synapse* syn_)
{
    mexErrMsgTxt("[CalciumSensor] cannot be added to a synapse\n");
}


void CalciumSensor::connect(conductance* cond_)
{
    mexErrMsgTxt("[CalciumSensor] cannot be added to a conductance\n");
}

void CalciumSensor::integrate(void)
{
    Ca_average += (dt/tau)*(comp->Ca - Ca_average);
}

void CalciumSensor::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[CalciumSensor] unsupported solver order\n");
    }
}


#endif
