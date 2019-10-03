// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Integral controller, as in O'Leary et al
// This controller can control either a synapse
// or a conductance

#ifndef FSDCONTROLLER
#define FSDCONTROLLER
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class FSDController: public mechanism {

protected:

    // pointer to the FSD target mechanism
    mechanism * target = nullptr;


    // housekeeping
    double delta_g = 0;
    double F_error = 0;
    double S_error = 0;
    double D_error = 0;

public:
    // scale and sign parameters
    double A = 0;
    double B = 0;
    double C = 0;

    // timescale
    double tau_g = 5000;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    FSDController(double A_, double B_, double C_, double tau_g_)
    {
        A = A_;
        B = B_;
        C = C_;
        tau_g = tau_g_;
    }


    void integrate(void);

    void checkSolvers(int);

    void connect(conductance *);
    void connect(synapse*);
    void connect(compartment*);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

};

string FSDController::getClass() {
    return "FSDController";
}

// unimplemented
double FSDController::getState(int idx) {
    return 0;
}

// the only thing we can return is the value of gbar
int FSDController::getFullStateSize(){return 1; }


int FSDController::getFullState(double *cont_state, int idx) {
    // return the current conductance level
    cont_state[idx] = channel->gbar;
    idx++;
    return idx;
}


void FSDController::connect(conductance * channel_) {
    // connect to a channel
    channel = channel_;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

    controlling_class = (channel_->getClass()).c_str();
}

void FSDController::connect(compartment* comp_) {
    mexErrMsgTxt("[FSDController] This mechanism cannot connect to a compartment object. You want to add a FSDTarget object to a compartment");
}

void FSDController::connect(synapse* syn_){
    mexErrMsgTxt("[FSDController] This mechanism cannot connect to a synapse object");
}


void FSDController::integrate(void) {

    // connect to the FSDtarget object if need be
    if (!target) {
        target = (channel->container)->getMechanismPointer("FSD");

        if (!target) {
            mexErrMsgTxt("Could not connect to FSDTarget\n");
        } else {
            // mexPrintf("Connected to FSDTarget\n");
        }
    }


    F_error = target->getState(0) - target->getState(3);
    S_error = target->getState(1) - target->getState(4);
    D_error = target->getState(2) - target->getState(5);

    // mexPrintf("D_error = %f\n",D_error);


    delta_g = (dt/tau_g)*(A*F_error + B*S_error + C*D_error)*channel->gbar_next;



    // only update if not negative or zero
    if (channel->gbar_next+delta_g > 0) {
        channel->gbar_next += delta_g;
    }
    else {
        mexPrintf("not updating because channels will be < 0\n");
    }

}



void FSDController::checkSolvers(int k) {
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[FSDController] unsupported solver order\n");
    }
}




#endif
