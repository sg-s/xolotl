// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Integral controller with Sushi-belt mRNA transport
// intended to work on straight, unbranched neurites 

#ifndef SUSHICONTROLLER
#define SUSHICONTROLLER
#include "../controller.hpp"

//inherit controller class spec
class SushiController: public controller {

public:

    int controller_idx; // keeps track of which controller this is in the compartment

    // specify parameters + initial conditions 
    SushiController(conductance* channel_, double tau_m_, double tau_g_, double m_, compartment* uc_, compartment* dc_, double A_, double B_, double C_)
    {
        channel = channel_; 
        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;
        upstream_compartment = uc_;
        downstream_compartment = dc_;
        A = A_;
        B = B_;
        C = C_;
    }
    
    void integrate(double Ca_error, double A, double dt);
    double get_gbar(void);

};

void SushiController::integrate(double Ca_error, double A, double dt)
{
    // figure out the mRNA in the upstream and downstream compartments
    double mdot;
    if (upstream_compartment == NULL && downstream_compartment != NULL) {
        // this is the soma
        double down_m = (downstream_compartment->getControllerPointer(controler_idx))->m;  
        mdot = Ca_error/tau_m - A*m + B*down_m; 
    } elseif (upstream_compartment != NULL && downstream_compartment == NULL) {
        // this is the process terminal 
        double up_m = (dupstream_compartment->getControllerPointer(controler_idx))->m;  
        mdot = A*m_up - B*m - C*m;
    } else {
        // this is a generic intermediate section of the neurite.
        double down_m = (downstream_compartment->getControllerPointer(controler_idx))->m; 
        double up_m = (dupstream_compartment->getControllerPointer(controler_idx))->m;
        mdot = A*m_up + B*m_down - (A+B+C)*m;
    }

    // integrate mRNA
    m += mdot;
    if (m < 0) {m = 0;} // make sure mRNA levels stay positive 

    // calculate conductance, not conductance density
    double g = (channel->gbar)*A;
    (channel->gbar) += ((dt/tau_g)*(m - g))/A;


}

double SushiController::get_gbar(void)
{
    return channel->gbar;
}


#endif
