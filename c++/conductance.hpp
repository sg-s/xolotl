// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
//Abstract class for defining conductances
// This class includes the following integration
// methods:
// exponential Euler (default)
// Runge-Kutta 4 (can be chosen using 
//      solver_order = 4)
// 
#ifndef CONDUCTANCE
#define CONDUCTANCE
#include <cmath>
#include <string>
// #include "mechanism.hpp"
using std::string;
class compartment;

class conductance {
protected:
    int V_idx = 0;
public:
    compartment *container; // pointer to compartment that contains this
    double gbar;
    double gbar_next;
    double g;
    double E;
    double m = 0;
    double h = 1;

    double verbosity = 0;

    int p = 1;
    int q = 0;

    double k_m[4] = {0,0,0,0};
    double k_h[4] = {0,0,0,0};

    double dt = .05;
    double temperature = 11;
    double temperature_ref = 11;


    // switches to tell xolotl
    // if channel supports approximation 
    // for performance speedup 
    int approx_m = 0;
    int approx_h  = 0;

    double m_inf_cache[200];
    double h_inf_cache[200];
    double tau_m_cache[200];
    double tau_h_cache[200];



    conductance()
    {
        container = 0; //null pointer for safety
    }

    ~conductance() {}

    virtual void integrate(double, double);
    virtual void integrateMS(int, double, double);

    virtual void connect(compartment*); 
    virtual string getClass(void) = 0;
    double getCurrent(double);
    void checkSolvers(int);

    double mdot(double, double, double);
    double hdot(double, double, double);

    virtual double m_inf(double, double);
    virtual double h_inf(double, double);
    virtual double tau_m(double, double);
    virtual double tau_h(double, double);

    inline double fast_pow(double, int);
    inline double fast_exp(double);

    void buildLUT(double);


    // housekeeping, temp variables
    double minf = 0;
    double hinf = 1;

};


// build look-up table
void conductance::buildLUT(double approx_channels)
{
    if (approx_channels == 0)
    {
        // turn off approximations
        approx_m = 0;
        approx_h = 0;
        return;
    }

    if (approx_m == 1)
    {
        if (verbosity > 0)
        {
            mexPrintf("%s using approximate activation functions\n", getClass().c_str());  
        }
        
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V,0);
            tau_m_cache[(int) round(V+99)] = tau_m(V,0);
        }
    }

    if (approx_h == 1) {
        if (verbosity > 0)
        {
           mexPrintf("%s using approximate in-activation functions\n", getClass().c_str()); 
        }
        
        for (double V = -99; V < 101; V++) {
            h_inf_cache[(int) round(V+99)] = h_inf(V,0);
            tau_h_cache[(int) round(V+99)] = tau_h(V,0);
        }

    }


}


// Exponential Euler integrator 
void conductance::integrate(double V, double Ca) {   


    V_idx = (int) round(V+99);
    if (V_idx < 0) {V_idx = 0;};
    if (V_idx > 200) {V_idx = 200;};

    // assume that p > 0
    switch (approx_m)
    {
        case 0:
            minf = m_inf(V,Ca);
            m = minf + (m - minf)*exp(-dt/tau_m(V,Ca));
            break;
        default:
            
            

            m = m_inf_cache[V_idx] + (m - m_inf_cache[V_idx])*fast_exp(-(dt/tau_m_cache[V_idx]));
            
            
            break;
    }
    


    g = gbar*fast_pow(m,p);

    switch (q)
    {
        case 0:
            break;
        default:

            switch (approx_h)
            {
                case 0:
                    hinf = h_inf(V,Ca);
                    h = hinf + (h - hinf)*exp(-dt/tau_h(V,Ca));
                    break;
                default:



                    h = h_inf_cache[V_idx] + (h - h_inf_cache[V_idx])*fast_exp(-(dt/tau_h_cache[V_idx]));
                    break;
            }

            g = g*fast_pow(h,q);            
            break;
    }

    gbar = gbar_next;

}

inline double conductance::fast_pow(double x, int a)
{
    switch (a)
    {
        case 0:
            return 1;
            break;
        case 1:
            return x;
            break;
        case 2:
            return x*x;
            break;
        case 3:
            return x*x*x;
            break;
        case 4:
            return x*x*x*x;
            break;
        case 5:
            return x*x*x*x*x;
            break;
        case 6:
            return x*x*x*x*x*x;
            break;
        case 7:
            return x*x*x*x*x*x*x;
            break;
        case 8:
            return x*x*x*x*x*x*x*x;
            break;
    }
    return x;
}


inline double conductance::fast_exp(double x) {
    x = 1.0 + x / 256.0;
    x *= x; x *= x; x *= x; x *= x;
    x *= x; x *= x; x *= x; x *= x;
    return x;
}


// Runge-Kutta 4 integrator 
void conductance::integrateMS(int k, double V, double Ca) {


    if (q == 0)
    {
        switch (k)
        {
            case 0:
                k_m[0] = dt*(mdot(V, Ca, m));
                g = gbar*fast_pow(m,p);
                break;
            case 1:
                k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
                g = gbar*fast_pow(m + k_m[0]/2,p);
                break;
            case 2:
                k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
                g = gbar*fast_pow(m + k_m[1]/2,p);
                break;
            case 3:
                k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
                g = gbar*fast_pow(m + k_m[2],p);
                break;
            case 4:
                // last step
                m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
                break;
        }
            


    } else {


        switch (k)
        {
            case 0:
                k_m[0] = dt*(mdot(V, Ca, m));
                k_h[0] = dt*(hdot(V, Ca, h));
                g = gbar*fast_pow(m,p)*fast_pow(h,q);
                break;
            case 1:
                k_m[1] = dt*(mdot(V, Ca, m + k_m[0]/2));
                k_h[1] = dt*(hdot(V, Ca, h + k_h[0]/2));
                g = gbar*fast_pow(m + k_m[0]/2,p)*fast_pow(h + k_h[0]/2,q);
                break;
            case 2:
                k_m[2] = dt*(mdot(V, Ca, m + k_m[1]/2));
                k_h[2] = dt*(hdot(V, Ca, h + k_h[1]/2));
                g = gbar*fast_pow(m + k_m[1]/2,p)*fast_pow(h + k_h[1]/2,q);
                break;
            case 3:
                k_m[3] = dt*(mdot(V, Ca, m + k_m[2]));
                k_h[3] = dt*(hdot(V, Ca, h + k_h[2]));
                g = gbar*fast_pow(m + k_m[2],p)*fast_pow(h + k_h[2],q);
                break;
            case 4:
                m = m + (k_m[0] + 2*k_m[1] + 2*k_m[2] + k_m[3])/6;
                h = h + (k_h[0] + 2*k_h[1] + 2*k_h[2] + k_h[3])/6;
                break;
        }

    }

    gbar = gbar_next;
}


double conductance::getCurrent(double V) { return g * (V - E); }

void conductance::connect(compartment *pcomp_) {
    container = pcomp_;
    gbar_next = gbar;
}

// asks each conductance if they have a solver with this order
void conductance::checkSolvers(int solver_order) {
    if (solver_order == 0){
        return;
    } else if (solver_order == 4) {
        return;
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
