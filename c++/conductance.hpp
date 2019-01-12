/*

This document describes the "conductance" C++ class.
This class describes objects that are conductances, or
populations of ion channels.

This is an abstract class, and concrete implementations
of ion channel types need to inherit from this class and
define certain attributes like their activation functions. 

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| yes |  nothing | compartment |

## Properties 

### `container`

| type | default | user-accessible |
| --------  | ------ | -------  |
| compartment* |  NULL | no |

### `gbar`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | yes |

The maximal conductance of this channel type (in $uS/mm^2$). This
is typically exposed to the user as a parameter to set and modify. 


### `gbar_next`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |

### `g`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |

The instantaneous conductance of this channel type. 
This is a product of `gbar` and the activation and 
inactivation variables. 

$$g = \bar{g} m^p h^q$$

### `E`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | yes |

The reversal potential of this channel type.

### `m`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |

The activation variable of this channel type.

### `h`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  1 | no |


The inactivation variable of this channel type.

### `verbosity`

| type | default | user-accessible |
| --------  | ------ | -------  |
| double |  0 | no |


A flag that tells this channel how verbose it should be.
This should not be exposed to the user, since it it 
broadcast to all components from `xolotl.verbosity`.

*/
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
    double E = 0;
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


/*
This method constructs a look up table (LUT)
that is used to estimate $m_{inf}$ and other
functions of the voltage. Since these functions 
are repeatedly evaluated, it is often faster to compute
them for some values of the voltage once, store these
values in a table, and use this table subsequently. 
This is an approximation since the voltage is rounded
off to the nearest value in the look-up table, uses
a little more memory, but can be much faster. 
*/
void conductance::buildLUT(double approx_channels) {
    if (approx_channels == 0) {
        // turn off approximations
        approx_m = 0;
        approx_h = 0;
        return;
    }

    if (approx_m == 1) {
        if (verbosity > 0) {
            mexPrintf("%s using approximate activation functions\n", getClass().c_str());  
        }
        
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V,0);
            tau_m_cache[(int) round(V+99)] = tau_m(V,0);
        }
    }

    if (approx_h == 1) {
        if (verbosity > 0) {
           mexPrintf("%s using approximate in-activation functions\n", getClass().c_str()); 
        }
        
        for (double V = -99; V < 101; V++) {
            h_inf_cache[(int) round(V+99)] = h_inf(V,0);
            tau_h_cache[(int) round(V+99)] = tau_h(V,0);
        }
    }
} // buildLUT


/*
This method integrates the conductance object using
the exponential Euler method. This is the default
integration method used by xolotl. If an exact solution
is to be calculated (i.e.,`approx_m = 0` and `approx_h=0`)
then `m` and `h` are updated using the exponential Euler
equation using function evaluations of the activation 
functions at this voltage and Calcium.

Otherwise, the lookup table is used to update `m` and `h`
in this channel. 

Note that this method is defined as virtual, so it can be
overridden by integration methods specified in a specific
conductance. 

**See Also** 

* [virtual methods in C++](http://www.cplusplus.com/doc/tutorial/polymorphism/)
*/
void conductance::integrate(double V, double Ca) {   


    V_idx = (int) round(V+99);
    if (V_idx < 0) {V_idx = 0;};
    if (V_idx > 200) {V_idx = 200;};

    // assume that p > 0
    switch (approx_m) {
        case 0:
            minf = m_inf(V,Ca);
            m = minf + (m - minf)*exp(-dt/tau_m(V,Ca));
            break;

        default:
            m = m_inf_cache[V_idx] + (m - m_inf_cache[V_idx])*fast_exp(-(dt/tau_m_cache[V_idx]));
            break;
    } // switch approx_m
    
    g = gbar*fast_pow(m,p);

    switch (q) {
        case 0:
            break;
        default:
            switch (approx_h) {
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
    } // switch q

    gbar = gbar_next;

}


/*

This method is a dirty hack to speed up computing
exponents in C++. This requires that the power that a 
number is raised to be an integer (0-8)

*/
inline double conductance::fast_pow(double x, int a) {
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

/*
This method constitutes a dirty hack which 
is a faster way to compute exp(x)
but is less precise
*/
inline double conductance::fast_exp(double x) {
    x = 1.0 + x / 256.0;
    x *= x; x *= x; x *= x; x *= x;
    x *= x; x *= x; x *= x; x *= x;
    return x;
}


/*

This method integrates a channel object using a multi-step
solver (MS = "multi-step"). The "sub-step" is indicated in 
the integer k, which is the first input to this method. 

The multi-step solver that is used here is a Runge-Kutta 4th
order solver. Thus, k can have values up to 4. 

Based on `k`, different elements of the arrays `k_m` and `k_h`
are calculated and stored. At each step, the derivative functions
`mdot` and `hdot` are computed. 

**See Also**

* [The Runge Kutta Method](https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods)

*/
void conductance::integrateMS(int k, double V, double Ca) {

    if (q == 0) {
        // q= 0 means channel does not inactivate
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

/*

The method returns the current that flows through
this channel at this moment. 

*/
double conductance::getCurrent(double V) { return g * (V - E); }

/*
This method "connects" a conductance object to a compartment
object. This sets the `container` property of the conductance,
so the channel knows which compartment contains it. 
*/
void conductance::connect(compartment *pcomp_) {
    container = pcomp_;
    gbar_next = gbar;
}

/*
*/
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

/*
This method defines the rate of change of the `m` variable
of this conductance. This definition is used when `integrateMS` is used. 
*/
double conductance::mdot(double V, double Ca, double m_) {
    return (m_inf(V,Ca) - m_)/tau_m(V,Ca);
}

/*
This method defines the rate of change of the `h` variable
of this conductance. This definition is used when `integrateMS` is used. 
*/
double conductance::hdot(double V, double Ca, double h_) {
    return (h_inf(V,Ca) - h_)/tau_h(V,Ca);
}

/*
This method defines the activation curve of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 
*/
double conductance::m_inf(double V, double Ca){return 0;}

/*
This method defines the inactivation curve of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 
*/
double conductance::h_inf(double V, double Ca){return 1;}

/*
This method defines the dependence of the timescale 
of the activation variable on the voltage of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 
*/
double conductance::tau_m(double V, double Ca){return 1;}

/*
This method defines the dependence of the timescale 
of the inactivation variable on the voltage of this channel.
This is a virtual method, and is meant to be defined in 
the channel object. 
*/
double conductance::tau_h(double V, double Ca){return 1;}

#endif
