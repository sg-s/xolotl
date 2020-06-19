// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: component storing a single calcium target
//

#ifndef CALCIUMTARGET
#define CALCIUMTARGET
#include "mechanism.hpp"
#include <limits>


//inherit mechanism class spec
class CalciumTarget: public mechanism {

protected:

    
public:

    // parameters for CalciumTarget
    double Target;


    // specify parameters + initial conditions 
    CalciumTarget(double Target_)
    {
        Target = Target_;
    }

    void checkSolvers(int);

    void integrate(void);


    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);


    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

};

string CalciumTarget::getClass() {
    return "CalciumTarget";
}


// we are abusing getState to read out value of the target 
// from other mechanisms 
double CalciumTarget::getState(int idx){return Target;}


int CalciumTarget::getFullStateSize(){return 0; }


int CalciumTarget::getFullState(double *cont_state, int idx) {
    return idx;
}

// connection methods
void CalciumTarget::connect(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}

void CalciumTarget::connect(conductance* cond_) {
    mexErrMsgTxt("[CalciumTarget] This mechanism cannot connect to a conductance object");
}

void CalciumTarget::connect(synapse* syn_) {
    mexErrMsgTxt("[CalciumTarget] This mechanism cannot connect to a synapse object");
}



void CalciumTarget::integrate(void) {
    // do nothing
}



void CalciumTarget::checkSolvers(int k) {
    return;
}



#endif
