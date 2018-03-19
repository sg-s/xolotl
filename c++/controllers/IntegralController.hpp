// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Integral controller, as in O'Leary et al 
// This controller can control either a synapse
// or a conductance 

#ifndef INTEGRALCONTROLLER
#define INTEGRALCONTROLLER
#include "../controller.hpp"

//inherit controller class spec
class IntegralController: public controller {

protected:
public:
    // timescales
    double tau_m;
    double tau_g; 

    // mRNA concentration 
    double m; 

    // specify parameters + initial conditions for 
    // controller that controls a conductance 
    IntegralController(double tau_m_, double tau_g_, double m_)
    {

        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;

        if (isnan (m)) { m = 0; }
    }

    
    void integrate(double Ca_error, double A, double dt);
    void connect(conductance * channel_, synapse * syn_);
    double get_gbar(void);
    double get_m(void);

};

void IntegralController::connect(conductance * channel_, synapse * syn_)
{
    if (channel_)
    {
        // connect to a channel
        channel = channel_;
    }
    if (syn_)
    {
        // connect to a synapse 
        syn = syn_;
    }
}

void IntegralController::integrate(double Ca_error, double A, double dt)
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
        double g = (channel->gbar)*A;
        (channel->gbar) += ((dt/tau_g)*(m - g))/A;

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
double IntegralController::get_m(void)
{
    return m;
}

// return the conductance of either the 
// channel or the synapse that this 
// controller is controlling 
double IntegralController::get_gbar(void)
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
