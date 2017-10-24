// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Integral controller, as in O'Leary et al 
#ifndef INTEGRALCONTROLLER
#define INTEGRALCONTROLLER
#include "../controller.hpp"

//inherit controller class spec
class IntegralController: public controller {

protected:
    double tau_m;
    double tau_g; 

public:

    int controller_idx; // keeps track of which controller this is in the compartment
    double m; 

    // specify parameters + initial conditions 
    IntegralController(conductance* channel_, double tau_m_, double tau_g_, double m_)
    {
        channel = channel_; 
        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;
    }
    
    void integrate(double Ca_error, double A, double dt);
    double get_gbar(void);

};

void IntegralController::integrate(double Ca_error, double A, double dt)
{
    // integrate mRNA
    m += (dt/tau_m)*(Ca_error);

    // calculate conductance, not conductance density
    double g = (channel->gbar)*A;
    (channel->gbar) += ((dt/tau_g)*(m - g))/A;


}

double IntegralController::get_gbar(void)
{
    return channel->gbar;
}


#endif
