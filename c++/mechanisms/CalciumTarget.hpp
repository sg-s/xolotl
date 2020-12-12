// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: component storing a single calcium target
//

#ifndef CALCIUMTARGET
#define CALCIUMTARGET
#include "mechanism.hpp"

//inherit mechanism class spec
class CalciumTarget: public mechanism {

protected:

    
public:

    // parameters for CalciumTarget
    double Target;


    // specify parameters + initial conditions 
    CalciumTarget(double Target_) {
        Target = Target_;
        name = "CalciumTarget";
    }

    void checkSolvers(int);

    void connectCompartment(compartment*);

    double getState(int);
    

};


// we are abusing getState to read out value of the target 
// from other mechanisms 
double CalciumTarget::getState(int idx){return Target;}



// connection methods
void CalciumTarget::connectCompartment(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}



// this works with any solver order, so let's say so
void CalciumTarget::checkSolvers(int k) {
    return;
}



#endif
