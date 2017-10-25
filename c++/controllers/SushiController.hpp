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

protected:
    double tau_m;
    double tau_g;
    
    compartment * upstream_compartment;
    compartment * downstream_compartment;

    double Alpha;
    double Beta;
    double Gamma;

public:

    double m;

    // specify parameters + initial conditions 
    SushiController(conductance* channel_, compartment* uc_, compartment* dc_, double tau_m_, double tau_g_, double m_,  double Alpha_, double Beta_, double Gamma_)
    {
        // pointers
        channel = channel_; 
        upstream_compartment = uc_;
        downstream_compartment = dc_;

        // inherit from integral control
        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;

        // sushi trafficking 
        Alpha = Alpha_;
        Beta = Beta_;
        Gamma = Gamma_;
    }
    
    void integrate(double Ca_error, double A, double dt);
    double get_gbar(void);
    double get_m(void);

};

void SushiController::integrate(double Ca_error, double A, double dt)
{
    // mexPrintf("this controller's m =  %f\n",this->m);
    // mexPrintf("pointer address of current controller =  %p\n",this);
    // figure out the mRNA in the upstream and downstream compartments
    double mdot = 0;
    if (upstream_compartment == NULL && downstream_compartment != NULL) {
        // this is the soma
        // mexPrintf("pointer address to what should be S1 =  %p\n",cp);
        double down_m = (downstream_compartment->getControllerPointer(controller_idx))->get_m(); 
        // mexPrintf("downstream m of soma =  %f\n",down_m);

        mdot = Ca_error/tau_m - Alpha*m + Beta*down_m - Gamma*m; 
        //mdot = 1 - Alpha*m + Beta*down_m - Gamma*m;  // 1 is the fixed transcription rate
    } else if (upstream_compartment != NULL && downstream_compartment == NULL) {
        // this is the process terminal 
        double up_m = (upstream_compartment->getControllerPointer(controller_idx))->get_m();  

        mdot = Alpha*up_m - Beta*m - Gamma*m;
    } else if (upstream_compartment != NULL && downstream_compartment != NULL) {
        // this is a generic intermediate section of the neurite.
        double up_m = (upstream_compartment->getControllerPointer(controller_idx))->get_m(); 
        double down_m = (downstream_compartment->getControllerPointer(controller_idx))->get_m(); 

        mdot = Alpha*up_m + Beta*down_m - (Alpha+Beta+Gamma)*m;
    } else {
        // there are no other compartments, and this should behave like a normal integral controller
        mexPrintf("falling back to normal integral control");
        mdot = Ca_error/tau_m;
    }

    // integrate mRNA
    m += mdot;
    if (m < 0) {m = 0;} // make sure mRNA levels stay positive 

    // calculate conductance, not conductance density
    double g = (channel->gbar)*A;
    (channel->gbar) += ((dt/tau_g)*(m - g))/A;


}


// Why do we need this bizarre construction? Why can't I just
// directly read m? it doesn't seem to work, which is why
// I'm doing this crazy bit of foolishness
// this just puts us deeper in the hole where controllers
// are forced to be one-dimensional systems with a state
// variable called m
double SushiController::get_m(void)
{
    return m;
}

double SushiController::get_gbar(void)
{
    return channel->gbar;
}


#endif
