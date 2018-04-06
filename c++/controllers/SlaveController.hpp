// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Slave Integral Controller
// modification of the Integral controller
// as in O'Leary 2014 

#ifndef SLAVECONTROLLER
#define SLAVECONTROLLER
#include "../controller.hpp"

//inherit controller class spec
class SlaveController: public controller {

protected:
    controller * master_controller;

public:
    // timescales
    double tau_m;
    double tau_g; 

    // mRNA concentration 
    double m; 

    // fudge factor
    double phi;


    // area of the container this is in
    // this is NOT necessarily the area of the compartment
    // that contains it
    double container_A;

    // specify parameters + initial conditions for 
    // controller that controls a conductance 
    SlaveController(double tau_m_, double tau_g_, double m_, double phi_)
    {

        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;
        phi = phi_;

        // Null pointers for safety
        master_controller = NULL;
        channel = NULL;
        syn = NULL;

        if (isnan (m)) { m = 0; }
        if (isnan (phi)) { phi = 1; }
    }

    
    void integrate(double Ca_error, double dt);
    void connect(conductance * channel_, synapse * syn_);
    double get_gbar(void);
    double get_m(void);

    void setMaster(controller * master_controller_);

};

void SlaveController::setMaster(controller * master_controller_)
{
    master_controller = master_controller_;
}

void SlaveController::connect(conductance * channel_, synapse * syn_)
{
    if (channel_)
    {
        // connect to a channel
        channel = channel_;

        // attempt to read the area of the container that this
        // controller should be in. note that this is not necessarily the
        // container that contains this controller. rather, it is 
        // the compartment that contains the conductnace/synapse 
        // that this controller controls
        container_A  = (channel->container)->A;
        container_vol  = (channel->container)->vol;
    }
    if (syn_)
    {
        // connect to a synapse 
        syn = syn_;
    }
}

void SlaveController::integrate(double Ca_error, double dt)
{
    // integrate mRNA
    m += (dt/tau_m)*((master_controller->get_m()) - m);

    // double master_m = master_controller->get_m();
    // // mexPrintf("master_m =  %f\n",master_m);
    // m += (dt/tau_m)*(master_m - m);

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
        (channel->gbar) += ((dt/tau_g)*(m*container_vol*phi - g))/container_A;

        // make sure it doesn't go below zero
        if ((channel->gbar) < 0) {
            (channel->gbar) = 0;
        }
    }

    if (syn) {
        // syn is a non-NULL pointer, so
        // this controller must be controlling a 
        // synpase that is presynaptic to this compartment 
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

// return the mRNA level, because this is a protected
// member 
double SlaveController::get_m(void)
{
    return m;
}

// return the conductance of either the 
// channel or the synapse that this 
// controller is controlling 
double SlaveController::get_gbar(void)
{

    double gbar;
    if (channel) {
        gbar = channel->gbar;
    }
    if (syn) {
        gbar = syn->gbar;
    }
    return gbar;
}


#endif
