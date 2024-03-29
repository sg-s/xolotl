// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: integral homeostatic controller to work with LiuSensor 
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//

#pragma once
#include "mechanism.hpp"
#include <limits>


//inherit mechanism class spec
class IntegralController: public mechanism {

protected:
    // pointer to sensor 
    mechanism* Fast = 0;
    mechanism* Slow = 0;
    mechanism* DC = 0;

    int sensor_connected = 0;
    
public:

    // parameters for IntegralController
    double tau = 5e3;
    double A = 0;
    double B = 0;
    double C = 0;
    double FTarget = 0;
    double STarget = 0;
    double DTarget = 0;

    // specify parameters + initial conditions 
    IntegralController(double tau_, double A_, double B_, double C_, double FTarget_, double STarget_, double DTarget_)
    {
        // wiring 
        tau = tau_;
        A = A_;
        B = B_;
        C = C_;
        FTarget = FTarget_;
        STarget = STarget_;
        DTarget = DTarget_;

        // defaults
        if (isnan(tau)) {tau = 5000; } // ms

        name = "IntegralController";

    }
    
    void integrate(void);
    void connectConductance(conductance*);
    void connectToLiuSensor(void);

};


void IntegralController::connectToLiuSensor() {


    int n_mech = (channel->container)->n_mech;

    for (int i = 0; i < n_mech; i++) {

        string this_mech = (channel->container)->getMechanismPointer(i)->name.c_str();

        if (this_mech == "FastSensor") {
            Fast = (channel->container)->getMechanismPointer(i);
            // mexPrintf("connected to fast sensor\n");
            sensor_connected++;
        }

        if (this_mech == "SlowSensor") {
            Slow = (channel->container)->getMechanismPointer(i);
            // mexPrintf("connected to SlowSensor\n");
            sensor_connected++;
        }

        if (this_mech == "DCSensor") {
            DC = (channel->container)->getMechanismPointer(i);
            // mexPrintf("connected to DC sensor\n");
            sensor_connected++;
        }
    }




}



// connection methods
void IntegralController::connect(conductance* cond_) {

    channel = cond_;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

    controlling_class = (channel->name).c_str();

    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (channel->container)->A;
}



void IntegralController::integrate(void) {
    switch (sensor_connected) {
        case 0:
            connectToLiuSensor();
            break;
        default:
            break;
    }


    double gdot = A*(FTarget - Fast->getState(0));
    gdot += B*(STarget - Slow->getState(0));
    gdot += C*(DTarget - DC->getState(0));
    gdot = gdot*(dt/tau);


    // negative conductances disallowed 
    if ((channel->gbar + gdot) > 0) {
        channel->gbar += gdot;
    }

}




