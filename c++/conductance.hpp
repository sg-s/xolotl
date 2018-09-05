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

    int supported_solver_order = 1;

public:
    compartment *container; // pointer to compartment that contains this
    double gbar;
    double g;
    double E;
    double m;
    double h;

    double dt;


    conductance()
    {
        container = 0; //null pointer for safety
    }

    ~conductance() {}

    virtual void integrate(double, double, double) = 0;
    //void integrateMS(double, double, double);
    void connect(compartment*); 
    virtual string getClass(void) = 0;
    double getCurrent(double);
    void checkSolvers(int);

};

double conductance::getCurrent(double V) { return g * (V - E); }

void conductance::connect(compartment *pcomp_) {container = pcomp_;}

void conductance::checkSolvers(int solver_order)
{
    if (solver_order == 0){return;}
    
    if (supported_solver_order % solver_order == 0){

    } else {
        mexPrintf("Error using %s", this->getClass().c_str());
        mexErrMsgTxt("Unsupported solver order \n");
    }

}


#endif
