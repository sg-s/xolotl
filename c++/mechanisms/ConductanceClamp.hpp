// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: conductance clamper
// clamps whatever conductance it is attached to to compartment->GPData

#pragma once
#include <limits>

//inherit controller class spec
class ConductanceClamp: public mechanism {

protected:


public:


    double Gain = 1;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    ConductanceClamp(double Gain_) {
        Gain = Gain_;
        fullStateSize = 1;
        name = "ConductanceClamp";
    }



    // fcn declarations
    void integrate(void);
    double getState(int);
    

};




double ConductanceClamp::getState(int idx) {
    switch (idx) {
        case 0:
            return channel->gbar;
            break;
        default:
            return std::numeric_limits<double>::quiet_NaN();
    }

}



void ConductanceClamp::integrate(void) {
    channel->gbar = (channel->container)->GPData;
}




