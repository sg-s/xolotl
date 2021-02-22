// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component source [O'Leary et al. 2014](https://www.sciencedirect.com/science/article/pii/S089662731400292X)
// component info: Integral controller of conductances and synapses

#ifndef INTEGRALCONTROLLER
#define INTEGRALCONTROLLER
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class IntegralController: public mechanism {

protected:
    // flag used to switch between
    // controlling channels and synapses
    // meaning:
    // 0 --- unset, will throw an error
    // 1 --- channels
    // 2 --- synapses
    int control_type = 0;

    // pointer to mechanism that generates the error
    // signal
    mechanism * feedback_error = 0;

public:
    // timescales
    double tau_m = std::numeric_limits<double>::infinity();
    double tau_g = 5e3;

    // mRNA concentration
    double m = 0;

    // area of the container this is in
    double container_A;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    IntegralController(double tau_m_, double tau_g_, double m_) {

        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;


        fullStateSize = 2;

        if (tau_g<=0) {mexErrMsgTxt("[IntegralController] tau_g must be > 0. Perhaps you meant to set it to Inf?\n");}

        name = "IntegralController";
    }


    void integrate(void);


    void init(void);

    void connectConductance(conductance*);
    void connectSynapse(synapse*);

    int getFullState(double * cont_state, int idx);
    double getState(int);
    

};


void IntegralController::init() {
    
    compartment* temp_comp;

    if (control_type == 1) {
        temp_comp = channel->container;
        
    } else if (control_type == 2) {
        temp_comp  = syn->post_syn;
    } else {
        mexErrMsgTxt("IntegralController can only control conductances or synapses\n");
    }

    int n_mech = temp_comp->n_mech;

    // read the Calcium Target from the CalciumTarget mechanism
    // in the compartment that the controlled object is in 
    for (int i = 0; i < n_mech; i++) {

        string this_mech_type = temp_comp->getMechanismPointer(i)->mechanism_type.c_str();
        string this_mech_name = temp_comp->getMechanismPointer(i)->name.c_str();

        if (this_mech_type == "target_error") {
            if (verbosity==0) {
                mexPrintf("IntegralController(%s) connected to ",controlling_class.c_str());
                mexPrintf("[%s]\n",this_mech_name.c_str());
            }

            feedback_error = temp_comp->getMechanismPointer(i);
        }
    }

    // attempt to read Ca_target from compartment -- legacy code support
    if (!feedback_error) {
        mexErrMsgTxt("Could not connect to any mechanism that identifies itself as a 'target_error' type.");
    }
}


double IntegralController::getState(int idx) {
    if (idx == 1) {return m;}
    else if (idx == 2) {return channel->gbar;}
    else {return std::numeric_limits<double>::quiet_NaN();}

}



int IntegralController::getFullState(double *cont_state, int idx) {
    // give it the current mRNA level
    cont_state[idx] = m;

    idx++;

    // and also output the current gbar of the thing
    // being controller
    if (channel) {
      cont_state[idx] = channel->gbar;
    }
    else if (syn) {
        cont_state[idx] = syn->gmax;
    }
    idx++;
    return idx;
}


void IntegralController::connectConductance(conductance * channel_) {

    // connect to a channel
    channel = channel_;
    comp = channel->container;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);



    controlling_class = (channel_->name).c_str();

    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (channel->container)->A;

    control_type = 1;


}


void IntegralController::connectSynapse(synapse* syn_) {

    // connect to a synapse
    syn = syn_;


    // make sure the compartment that we are in knows about us
    (syn->post_syn)->addMechanism(this);


    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (syn->post_syn)->A;

    control_type = 2;

}


void IntegralController::integrate(void) {



    switch (control_type) {
        case 0:
            mexErrMsgTxt("[IntegralController] mis-configured controller. Make sure this object is contained by a conductance or synapse object");
            break;


        case 1:

            {

            // integrate mRNA
            m += (dt/tau_m)*(feedback_error->getState(0));

            // mRNA levels below zero don't make any sense
            if (m < 0) {m = 0;}

            // copy the protein levels from this channel
            double gdot = ((dt/tau_g)*(m - channel->gbar*container_A));

            // make sure it doesn't go below zero
            if (channel->gbar + gdot/container_A < 0) {
                channel->gbar = 0;
            } else {
                channel->gbar += gdot/container_A;
            }


            break;

            }
        case 2:
            {


            // integrate mRNA
            m += (dt/tau_m)*(feedback_error->getState(0));

            // mRNA levels below zero don't make any sense
            if (m < 0) {m = 0;}

            // copy the protein levels from this syn
            double gdot = ((dt/tau_g)*(m - syn->gmax*1e-3));

            // make sure it doesn't go below zero
            if (syn->gmax + gdot*1e3 < 0) {
                syn->gmax = 0;
            } else {
                syn->gmax += gdot*1e3;
            }


            break;

            }

        default:
            mexErrMsgTxt("[IntegralController] mis-configured controller");
            break;

    }


}




#endif
