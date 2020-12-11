// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: homeostatic controller of maximal conductance 
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//

#ifndef LIUCONTROLLER
#define LIUCONTROLLER
#include "mechanism.hpp"
#include <limits>


//inherit mechanism class spec
class LiuController: public mechanism {

protected:
    // pointer to sensors
    mechanism* Fast = 0;
    mechanism* Slow = 0;
    mechanism* DC = 0;

    double FTarget = 0;
    double STarget = 0;
    double DCTarget = 0;


public:

    // parameters for LiuController
    double tau = 5e3;
    double A = 0;
    double B = 0;
    double C = 0;
    

    // specify parameters + initial conditions 
    LiuController(double tau_, double A_, double B_, double C_)
    {
        // wiring 
        tau = tau_;
        A = A_;
        B = B_;
        C = C_;

        // defaults
        if (isnan(tau)) {tau = 5000; } // ms



    }



    void integrate(void);
   
    void connectConductance(conductance*);


    double boltzmann(double);


    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    string getClass(void);

    void init(void);

};


void LiuController::init() {


    int n_mech = (channel->container)->n_mech;

    for (int i = 0; i < n_mech; i++) {

        string this_mech = (channel->container)->getMechanismPointer(i)->getClass().c_str();

        if (this_mech == "FastSensor") {
            Fast = (channel->container)->getMechanismPointer(i);
            if (verbosity==0) {
                mexPrintf("LiuController(%s) connected to [FastSensor]\n",controlling_class.c_str());
            }
        }

        if (this_mech == "SlowSensor") {
            Slow = (channel->container)->getMechanismPointer(i);
            if (verbosity==0) {
                mexPrintf("LiuController(%s) connected to [SlowSensor]\n",controlling_class.c_str());
            }
        }

        if (this_mech == "DCSensor") {
            DC = (channel->container)->getMechanismPointer(i);
            if (verbosity==0) {
                mexPrintf("LiuController(%s) connected to [DCSensor]\n",controlling_class.c_str());
            }
        }

        if (this_mech == "FSDTarget") {
            if (verbosity==0) {
                mexPrintf("LiuController(%s) connected to [FSDTarget]\n",controlling_class.c_str());
            }
            FTarget = (channel->container)->getMechanismPointer(i)->getState(0);
            STarget = (channel->container)->getMechanismPointer(i)->getState(1);
            DCTarget = (channel->container)->getMechanismPointer(i)->getState(2);
        }
    }




}

string LiuController::getClass() {
    return "LiuController";
}



int LiuController::getFullStateSize(){return 1; }


int LiuController::getFullState(double *cont_state, int idx) {
    cont_state[idx] = channel->gbar;
    idx++;
    return idx;
}

// connection methods
void LiuController::connectConductance(conductance* cond_) {

    channel = cond_;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

    controlling_class = (channel->getClass()).c_str();

    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (channel->container)->A;
}


void LiuController::integrate(void) {


    double gdot = A*(FTarget - Fast->getState(0));
    gdot += B*(STarget - Slow->getState(0));
    gdot += C*(DCTarget - DC->getState(0));
    gdot = gdot*(dt/tau)*(channel->gbar);


    // negative conductances disallowed 
    if ((channel->gbar + gdot) > 0) {
        channel->gbar += gdot;
    }

}





#endif
