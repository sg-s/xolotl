// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: instantaneous difference from calcium target of  intracellular calcium
// component source: [Gorur-Shandilya et al. 2020](https://www.nature.com/articles/s41598-020-72977-6)

#ifndef INSTCALCIUMERROR
#define INSTCALCIUMERROR
#include <limits>

//inherit mechanism class spec
class InstCalciumError: public mechanism {

protected:
public:


    double Target = 0;

    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    InstCalciumError(double Target_) {
        Target = Target_;

        fullStateSize = 1;

        name = "InstCalciumError";
        mechanism_type = "CalciumError";
    }

    double getState(int);
};



double InstCalciumError::getState(int i_) {
    (void) i_;
    // no matter what is asked, we return the same thing
    return  Target - (comp->Ca_prev);
}




#endif
