// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Integral controller, as in O'Leary et al 
// This controller can control either a synapse
// or a conductance 

#ifndef INTEGRALCONTROLLER
#define INTEGRALCONTROLLER
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class IntegralController: public mechanism {

protected:
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
    IntegralController(double tau_m_, double tau_g_, double m_)
    {

        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;


        if (tau_m<=0) {mexErrMsgTxt("[IntegralController] tau_m must be > 0. Perhaps you meant to set it to Inf?\n");}
        if (tau_g<=0) {mexErrMsgTxt("[IntegralController] tau_g must be > 0. Perhaps you meant to set it to Inf?\n");}
    }

    
    void integrate(void);

    void checkSolvers(int);

    void connect(conductance *);
    void connect(synapse*);
    void connect(compartment*);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

};


double IntegralController::getState(int idx)
{
    if (idx == 1) {return m;}
    else if (idx == 2) {return channel->gbar;}
    else {return std::numeric_limits<double>::quiet_NaN();}

}


int IntegralController::getFullStateSize(){return 2; }


int IntegralController::getFullState(double *cont_state, int idx)
{
    // give it the current mRNA level
    cont_state[idx] = m;

    idx++;

    // and also output the current gbar of the thing
    // being controller
    if (channel)
    {
      cont_state[idx] = channel->gbar;  
    }
    else if (syn)
    {
        cont_state[idx] = syn->gbar;  
    }
    idx++;
    return idx;
}


void IntegralController::connect(conductance * channel_)
{

    // connect to a channel
    channel = channel_;


    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

    // check that the compartment we are in has a valid
    // (non NaN Ca_target)

    controlling_class = (channel_->getClass()).c_str();

    // attempt to read the area of the container that this
    // controller should be in. note that this is not necessarily the
    // container that contains this controller. rather, it is 
    // the compartment that contains the conductance/synapse 
    // that this controller controls
    container_A  = (channel->container)->A;


}

void IntegralController::connect(compartment* comp_)
{
    mexErrMsgTxt("[IntegralController] This mechanism cannot connect to a compartment object");
}

void IntegralController::connect(synapse* syn_)
{
    mexErrMsgTxt("[IntegralController] This mechanism cannot connect to a synapse object");
}


void IntegralController::integrate(void)
{

    // if the target is NaN, we will interpret this
    // as the controller being disabled 
    // and do nothing 
    if (isnan((channel->container)->Ca_target)) {return;}

    double Ca_error = (channel->container)->Ca_target - (channel->container)->Ca_prev;


    // integrate mRNA
    m += (dt/tau_m)*(Ca_error);

    // mRNA levels below zero don't make any sense
    if (m < 0) {m = 0;}

    // copy the protein levels from this channel
    double g = channel->gbar*container_A;

    g += ((dt/tau_g)*(m - g));

    // make sure it doesn't go below zero
    if (g < 0) {g = 0;}

    // update
    channel->gbar_next = g/container_A;

}



void IntegralController::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech1] unsupported solver order\n");
    }
}




#endif
