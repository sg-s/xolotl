// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
//Abstract class for defining conductances
#ifndef CONDUCTANCE
#define CONDUCTANCE
#include <cmath>
#include <string>
// #include "mechanism.hpp"
using std::string;
class compartment;
// class controller;

class conductance {
protected:


public:
    compartment *container; // pointer to compartment that contains this
    double gbar;
    double g;
    double E;
    double m;
    double h;

    // temperature compensation parameters
    double Q_g;
    double Q_tau_m;
    double Q_tau_h;
    double delta_temp;

    conductance()
    {
        container = 0; //null pointer for safety
    }

    ~conductance() {}

    virtual void integrate(double, double, double, double) = 0;
    void connect(compartment*); // null pointer for safety
    virtual string getClass(void) = 0;
    double getCurrent(double);

};

double conductance::getCurrent(double V) { return g * (V - E); }

void conductance::connect(compartment *pcomp_) {container = pcomp_;}

#endif
