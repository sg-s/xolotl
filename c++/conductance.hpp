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
    void connect(compartment*); 
    virtual string getClass(void) = 0;
    double getCurrent(double);
    void checkSolvers(int);

    double mdot(double, double);
    double hdot(double, double);

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);




};

double conductance::getCurrent(double V) { return g * (V - E); }

void conductance::connect(compartment *pcomp_) {container = pcomp_;}

// asks each conductance if they have a solver with this order
void conductance::checkSolvers(int solver_order)
{
    if (solver_order == 0){return;}
    
    if (supported_solver_order % solver_order == 0){
        // make the vectors to store 
        // intermediate values
        std::vector<int> k_m(solver_order);
        std::vector<int> k_h(solver_order);

    } else {
        mexPrintf("Error using %s", getClass().c_str());
        mexErrMsgTxt("Unsupported solver order \n");
    }
}



double conductance::mdot(double V, double Ca)
{
    return (m_inf(V,Ca) - m)/tau_m(V,Ca);
}

double conductance::hdot(double V, double Ca)
{
    return (h_inf(V,Ca) - h)/tau_h(V,Ca);
}

// placeholder functions, these should be ovewritten
// as needed
double conductance::m_inf(double V, double Ca){return 0;}
double conductance::h_inf(double V, double Ca){return 1;}
double conductance::tau_m(double V, double Ca){return 1;}
double conductance::tau_h(double V, double Ca){return 1;}

#endif
