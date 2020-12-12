// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: component storing fast, slow, DC targets
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//

#ifndef FSDTARGET
#define FSDTARGET
#include "mechanism.hpp"
#include <limits>


//inherit mechanism class spec
class FSDTarget: public mechanism {

protected:

    
public:

    // parameters for FSDTarget
    double DCTarget;
    double STarget;
    double FTarget;


    // specify parameters + initial conditions 
    FSDTarget(double FTarget_, double STarget_, double DCTarget_)
    {
        // wiring 
        FTarget = FTarget_;
        STarget = STarget_;
        DCTarget = DCTarget_;

        name = "FSDTarget";
    }

    void checkSolvers(int);


    void connectCompartment(compartment*);
  


    double getState(int);
    

};


// we are abusing getState to read out value of the target 
// from other mechanisms 
double FSDTarget::getState(int idx){
    switch (idx) {
        case 0:
            return FTarget;
            break;
        case 1:
            return STarget;
            break;
        case 2:
            return DCTarget;
            break;
        default:
            return -1;
            break;
    }
}



// connection methods
void FSDTarget::connectCompartment(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}





void FSDTarget::checkSolvers(int k) {
    return;
}



#endif
