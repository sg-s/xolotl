// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Integral controller, as in O'Leary et al 
#ifndef INTCONTROLLER
#define INTCONTROLLER
#include "../controller.hpp"

//inherit controller class spec
class IntController: public controller {

public:

    // specify parameters + initial conditions 
    IntController(conductance* channel_, double tau_m_, double tau_g_, double G_, double m_)
    {
        channel = channel_; 
        tau_m = tau_m_;
        tau_g = tau_g_;
        G = G_;
        m = m_;
    }
    
    void integrate(double Ca_error, double dt);

};

void IntController::integrate(double Ca_error, double dt)
{
    // integrate mRNA
    m += (dt/tau_m)*(Ca_error);

    // integrate channels 
    // mexPrintf("tau_g is =  %f\n",tau_g);
    // mexPrintf("G is =  %f\n",G);
    // mexPrintf("m is =  %f\n",m);
    (channel->gbar) += (dt/tau_g)*(G*m - (channel->gbar));


}


#endif
