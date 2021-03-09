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

    mechanism* calcium_error; 

public:
    // timescales
    double tau_m = std::numeric_limits<double>::infinity();
    double tau_g = 5e3;

    // mRNA concentration
    double m = 0;

    // area of the container this is in
    double container_A;


    // gain term on the calcium error
    double Gain = 1;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    IntegralController(double tau_m_, double tau_g_, double m_, double Gain_) {

        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;
        Gain = Gain_;


        fullStateSize = 2;

        if (tau_g<=0) {mexErrMsgTxt("[IntegralController] tau_g must be > 0. Perhaps you meant to set it to Inf?\n");}

        if (isnan(m)) {m = 0;}

        name = "IntegralController";
        mechanism_type = "Homeostatic_regulator";
    }



    // fcn declarations
    void integrate(void);
    void init(void);
    double getState(int);
    

};


void IntegralController::init() {


    // attempt to read the area of the container that this
    // controller should be in.
    container_A  = (channel->container)->A;

    if (isnan(m)) {
        m = channel->gbar*(container_A);
    }


    // connect to CalciumError type mechanisms
    vector<mechanism*> temp = findMechanismsOfType("CalciumError");
    calcium_error = temp.at(0);

    // connect to any mechanism that is controlling the same conductance as this controller
    temp = findMechanismsControlling(controlling_class.c_str());

    // mexPrintf("Number of other mechanisms controlling the same thing = %i\n",temp.size());

    RHS_terms.insert(RHS_terms.end(), temp.begin(), temp.end());


    if (verbosity == 0) {
        mexPrintf("Number of additional RHS terms = %i \n",RHS_terms.size());
    }
    
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



void IntegralController::integrate(void) {


    RHS = 0;


    // get the RHS term due to the calcium error,
    // and multiply by gain
    if (calcium_error) {
        RHS += calcium_error->getPrevState(0)*Gain;
    }

    // there may be other RHS terms, get them all
    for (int i = 0; i < RHS_terms.size(); i++) {
        RHS += RHS_terms.at(i)->getPrevState(0);
    }


    // mexPrintf("IntegralController::integrate starting@131\n");

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
