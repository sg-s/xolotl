// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Integral controller, as in O'Leary et al 
// This controller can control either a synapse
// or a conductance 

#ifndef INTEGRALCONTROLLER
#define INTEGRALCONTROLLER
#include "controller.hpp"
#include <limits>

//inherit controller class spec
class IntegralController: public controller {

protected:
public:
    // timescales
    double tau_m;
    double tau_g; 

    // mRNA concentration 
    double m; 

    // area of the container this is in
    // this is NOT necessarily the area of the compartment
    // that contains it
    double container_A;

    // specify parameters + initial conditions for 
    // controller that controls a conductance 
    IntegralController(double tau_m_, double tau_g_, double m_)
    {

        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;

        if (isnan (m)) { m = 0; }
    }

    
    void integrate(double Ca_error, double dt);
    void connect(conductance * channel_, synapse * syn_);
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


int IntegralController::getFullStateSize()
{
    return 2; 
}


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


void IntegralController::connect(conductance * channel_, synapse * syn_)
{
    if (channel_)
    {
        // connect to a channel
        channel = channel_;

        controlling_class = (channel_->getClass()).c_str();

        // attempt to read the area of the container that this
        // controller should be in. note that this is not necessarily the
        // container that contains this controller. rather, it is 
        // the compartment that contains the conductance/synapse 
        // that this controller controls
        container_A  = (channel->container)->A;
    }
    if (syn_)
    {
        // connect to a synapse 
        syn = syn_;
    }
}

void IntegralController::integrate(double Ca_error, double dt)
{
    // integrate mRNA
    m += (dt/tau_m)*(Ca_error);

    // mexPrintf("m =  %f\n",m);

    // mRNA levels below zero don't make any sense
    if (m < 0) {
        m = 0;
    }




    if (channel) {
        // channel is a non-NULL pointer, so
        // this controller must be controlling a 
        // channel
        // calculate conductance, not conductance density
        
        double g = (channel->gbar)*container_A;
        (channel->gbar) += ((dt/tau_g)*(m - g))/container_A;

        // make sure it doesn't go below zero
        if ((channel->gbar) < 0) {
            (channel->gbar) = 0;
        }
    }

    if (syn) {
        // syn is a non-NULL pointer, so
        // this controller must be controlling a 
        // synapse that is presynaptic to this compartment 
        // calculate conductance, not conductance density
        double g = (syn->gbar); 

        // remember, that since everything is confusing,
        // synapses are in nS, and conductances are in 
        // uS/mm^2

        (syn->gbar) += ((dt/tau_g)*(m - g));

        // make sure it doesn't go below zero
        if ((syn->gbar) < 0) {
            (syn->gbar) = 0;
        }
    }


}



#endif
