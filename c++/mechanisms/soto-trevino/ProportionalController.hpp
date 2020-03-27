// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
//
// component info: Proportional controller of synapse strengths 
// component source: [Soto-Trevino et al 2001](https://www.nature.com/articles/nn0301_297)
// 
//
#ifndef PROPORTIONALCONTROLLER
#define PROPORTIONALCONTROLLER
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class ProportionalController: public mechanism {

protected:
    // flag used to switch between
    // controlling channels and synapses
    // meaning:
    // 0 --- unset, will throw an error
    // 1 --- channels
    // 2 --- synapses
    int control_type = 0;
public:
    // timescales
    double tau_m = std::numeric_limits<double>::infinity();

    // mRNA concentration
    // exists purely for compatibility with Integral Controller
    double m = 0;

    // ignored variable -- exists for compatibility
    double tau_g = 0;

    // area of the container this is in
    double container_A;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    ProportionalController(double tau_m_, double m_, double tau_g_)
    {
        tau_g = tau_g_;
        m = m_;
        tau_m = tau_m_;
        if (isnan(tau_m)) {tau_m = 10e3;};
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

string ProportionalController() {
    return "ProportionalController";
}


double ProportionalController::getState(int idx) {
    if (idx == 1) {return m;}
    else if (idx == 2) {return channel->gbar;}
    else {return std::numeric_limits<double>::quiet_NaN();}

}


int ProportionalController::getFullStateSize(){return 1; }


int ProportionalController::getFullState(double *cont_state, int idx) {

    // and also output the current gbar of the thing
    // being controller
    if (channel)
    {
      cont_state[idx] = channel->gbar;
    }
    else if (syn)
    {
        cont_state[idx] = syn->gmax;
    }
    idx++;
    return idx;
}


void ProportionalController::connect(conductance * channel_) {

    // connect to a channel
    channel = channel_;


    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

    controlling_class = (channel_->getClass()).c_str();

    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (channel->container)->A;

    control_type = 1;

}

void ProportionalController::connect(compartment* comp_) {
    mexErrMsgTxt("[ProportionalController] This mechanism cannot connect to a compartment object");
}

void ProportionalController::connect(synapse* syn_) {

    // connect to a synpase
    syn = syn_;


    // make sure the compartment that we are in knows about us
    (syn->post_syn)->addMechanism(this);


    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (syn->post_syn)->A;

    control_type = 2;

}


void ProportionalController::integrate(void) {


    switch (control_type)
    {
        case 0:
            mexErrMsgTxt("[ProportionalController] misconfigured controller. Make sure this object is contained by a conductance or synapse object");
            break;


        case 1:

            {
            // if the target is NaN, we will interpret this
            // as the controller being disabled
            // and do nothing
            if (isnan((channel->container)->Ca_target)) {return;}

            double Ca_error = (channel->container)->Ca_target - (channel->container)->Ca_prev;

            double g = (channel->gbar)*container_A;

            double gdot = (dt/tau_m)*(Ca_error)*g/((channel->container)->Ca_target);

            // make sure it doesn't go below zero
            if (channel->gbar_next + gdot < 0) {
                channel->gbar_next = 0;
            } else {
                channel->gbar_next += gdot/container_A;
            }


            break;

            }
        case 2:
            {
            // if the target is NaN, we will interpret this
            // as the controller being disabled
            // and do nothing

            if (isnan((syn->post_syn)->Ca_target)) {return;}

            double Ca_error = (syn->post_syn)->Ca_prev - (syn->post_syn)->Ca_target;

            double g = (syn->gmax);

            double gdot = (dt/tau_m)*(Ca_error)*g/((syn->post_syn)->Ca_target);


            // make sure it doesn't go below zero
            if (syn->gmax + gdot*1e3 < 0) {
                syn->gmax = 0;
            } else {
                syn->gmax += gdot*1e3;
            }


            break;

            }

        default:
            mexErrMsgTxt("[ProportionalController] misconfigured controller");
            break;

    }


}



void ProportionalController::checkSolvers(int k) {
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[ProportionalController] unsupported solver order\n");
    }
}




#endif
