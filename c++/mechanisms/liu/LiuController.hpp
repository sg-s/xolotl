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
    // pointer to sensor 
    mechanism* Fast = 0;
    mechanism* Slow = 0;
    mechanism* DC = 0;

    int sensor_connected = 0;
    
public:

    // parameters for LiuController1
    double tau = 5e3;
    double A = 0;
    double B = 0;
    double C = 0;
    double FTarget = 0;
    double STarget = 0;
    double DTarget = 0;

    // specify parameters + initial conditions 
    LiuController(double tau_, double A_, double B_, double C_, double FTarget_, double STarget_, double DTarget_)
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



    }

    void checkSolvers(int);

    void integrate(void);


    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    double boltzmann(double);


    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

    void connectToLiuSensor(void);

};


void LiuController::connectToLiuSensor() {


    int n_mech = (channel->container)->n_mech;

    for (int i = 0; i < n_mech; i++) {

        string this_mech = (channel->container)->getMechanismPointer(i)->getClass().c_str();

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

string LiuController::getClass() {
    return "LiuController";
}


double LiuController::getState(int idx){return 0;}


int LiuController::getFullStateSize(){return 0; }


int LiuController::getFullState(double *cont_state, int idx) {
    return idx;
}

// connection methods
void LiuController::connect(conductance* cond_) {

    channel = cond_;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

    controlling_class = (channel->getClass()).c_str();

    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (channel->container)->A;
}

void LiuController::connect(compartment* cond_) {
    mexErrMsgTxt("[LiuController] This mechanism cannot connect to a compartment object");
}

void LiuController::connect(synapse* syn_) {
    mexErrMsgTxt("[LiuController] This mechanism cannot connect to a synapse object");
}



void LiuController::integrate(void) {
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
    gdot = gdot*(dt/tau)*(channel->gbar);


    // negative conductances disallowed 
    if ((channel->gbar + gdot) > 0) {
        channel->gbar += gdot;
    }

}

// activation/inactivation functions
double LiuController::boltzmann(double x) {
    return 1/(1 + exp(x));
}



void LiuController::checkSolvers(int k) {
    if (k == 0) {return;}
    else {mexErrMsgTxt("[LiuController] unsupported solver order\n");}
}



#endif
