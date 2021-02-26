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

    // this keeps track of the total RHS value on the MRNA ODE
    double RHS = 0;

    // we use a vector to keep pointers to mechanisms that 
    // contribute to RHS terms in the mRNA ODE
    vector<mechanism*> RHS_terms;

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

        if (isnan(m)) {m = 0;}

        name = "IntegralController";
    }



    // fcn declarations
    void integrate(void);
    void init(void);
    void connectConductance(conductance*);
    double getState(int);
    

};


void IntegralController::init() {

    if (isnan(m)) {
        m = channel->gbar*(channel->container->A);
    }


    // connect to CalciumError type mechanisms
    vector<mechanism*> temp = findMechanismsOfType("CalciumError");
    RHS_terms.insert(RHS_terms.end(), temp.begin(), temp.end());

    // connect to any mechanism that is controlling the same conductance as this controller
    temp = findMechanismsControlling(this->controlling_class.c_str());
    RHS_terms.insert(RHS_terms.end(), temp.begin(), temp.end());
}


double IntegralController::getState(int idx) {
    switch (idx) {
        case 0:
            return m;
            break;
        case 1:
            return channel->gbar;
            break;
        default:
            return std::numeric_limits<double>::quiet_NaN();
    }

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

}



void IntegralController::integrate(void) {


    RHS = 0;
    for (int i = 0; i < RHS_terms.size(); i++) {
        RHS += RHS_terms[i]->getPrevState(0);
    }

    if (RHS == 0) {
        return;
    }

    // integrate mRNA
    m += (dt/tau_m)*(RHS);

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


}




#endif
