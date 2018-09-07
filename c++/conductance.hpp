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

class conductance {
protected:

    int supported_solver_order = 1;

public:
    compartment *container; // pointer to compartment that contains this
    double gbar;
    double g;
    double E;
    double m = 0;
    double h = 1;

    bool is_calcium = false;

    int p = 1;
    int q = 0;

    double k_m[4] = {0,0,0,0};
    double k_h[4] = {0,0,0,0};

    double dt;
    double temperature;
    double temperature_ref = 11;


    conductance()
    {
        container = 0; //null pointer for safety
    }

    ~conductance() {}

    virtual void integrate(double, double);
    virtual void integrateMS(int, double, double);

    void connect(compartment*); 
    virtual string getClass(void) = 0;
    double getCurrent(double);
    void checkSolvers(int);

    double mdot(double, double, double);
    double hdot(double, double, double);

    virtual double m_inf(double, double);
    virtual double h_inf(double, double);
    virtual double tau_m(double, double);
    virtual double tau_h(double, double);


};

// Exponential Euler integrator 
void conductance::integrate(double V, double Ca)
{   
    
    // assume that p > 0
    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    if (q == 0)
    {
        g = gbar*pow(m,p);
    } else {
        h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
        g = gbar*pow(m,p)*pow(h,q);
    }
    
}


// Runge-Kutta 4 integrator 
void conductance::integrateMS(int k, double V, double Ca)
{


    if (q == 0)
    {
        if (k == 0) {
            k_m[0] = dt*(mdot(V, Ca, m));
            g = gbar*pow(m,p);
        } else if (k == 1) {
            k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
            g = gbar*pow(m + k_m[0]/2,p);

        } else if (k == 2) {
            k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
            g = gbar*pow(m + k_m[1]/2,p);

        } else if (k == 3) {
            k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
            g = gbar*pow(m + k_m[2],p);

        } else {
            // last step
            m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
        }

    } else {

        // mexPrintf("conductance::integrateMS, p =  %i\n",p);
        if (k == 0) {
            k_m[0] = dt*(mdot(V, Ca, m));
            k_h[0] = dt*(hdot(V, Ca, h));
            g = gbar*pow(m,p)*pow(h,q);
        } else if (k == 1) {
            k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
            k_h[1] = dt*(hdot(V, Ca, h + k_h[0]/2));
            g = gbar*pow(m + k_m[0]/2,p)*pow(h + k_h[0]/2,q);

        } else if (k == 2) {
            k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
            k_h[2] = dt*(hdot(V, Ca, h + k_h[1]/2));
            g = gbar*pow(m + k_m[1]/2,p)*pow(h + k_h[1]/2,q);

        } else if (k == 3) {
            k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
            k_h[3] = dt*(hdot(V, Ca, h + k_h[2]));
            g = gbar*pow(m + k_m[2],p)*pow(h + k_h[2],q);

        } else {
            // last step
            m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
            h = h + (k_h[0] + 2*k_h[1] + 2*k_h[2] + k_h[3])/6;
        }

    }

    
    // mexPrintf("g = %f\n", g);

}


double conductance::getCurrent(double V) { return g * (V - E); }

void conductance::connect(compartment *pcomp_) {container = pcomp_;}

// asks each conductance if they have a solver with this order
void conductance::checkSolvers(int solver_order)
{
    if (solver_order == 0){return;}
    
    if (supported_solver_order % solver_order == 0){
        // make the vectors to store 
        // intermediate values


    } else {
        mexPrintf("Error using %s", getClass().c_str());
        mexErrMsgTxt("Unsupported solver order \n");
    }
}

double conductance::mdot(double V, double Ca, double m_)
{
    return (m_inf(V,Ca) - m_)/tau_m(V,Ca);
}

double conductance::hdot(double V, double Ca, double h_)
{
    return (h_inf(V,Ca) - h_)/tau_h(V,Ca);
}

// placeholder functions, these should be ovewritten
// as needed
double conductance::m_inf(double V, double Ca){return 0;}
double conductance::h_inf(double V, double Ca){return 1;}
double conductance::tau_m(double V, double Ca){return 1;}
double conductance::tau_h(double V, double Ca){return 1;}

#endif
