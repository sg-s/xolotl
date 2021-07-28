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
| int |  -1 | no |


A flag that tells this channel how verbose it should be.
This is not be exposed to the user, since it it
broadcast to all components from `xolotl.verbosity`.

*/
#pragma once

#include <cmath>
#include <string>
#include <random>
using std::string;
class compartment;


struct Permeability {
    double K = 0;
    double Ca = 0;
    double Na = 0;
    double Cl = 0;
};

class conductance {
protected:
    int V_idx = 0;

    // these integer flags check to see if 
    // tau_m or tau_h are 0, and if so, 
    // m and h are set to their ss values
    int instantaneous_m = 0;
    int instantaneous_h = 0;

public:
    compartment *container; // pointer to compartment that contains this
    double gbar;
    double g;
    double E = 0;
    double m = 0;
    double h = 1;

    string name = "unset";

    int verbosity = -1;

    int p = 1;
    int q = 0;

    double k_m[4] = {0,0,0,0};
    double k_h[4] = {0,0,0,0};

    double dt = .05;
    double temperature = 11;
    double temperature_ref = 11;


    // how many channels are we modelling?
    double unitary_conductance = 20e-6; // uS
    int N = 1;

    // switches to tell xolotl
    // if channel supports approximation 
    // for performance speedup 
    bool AllowMInfApproximation = true;
    bool AllowHInfApproximation = true;


    // these integer values are used to determine
    // whether the approximation is actually used
    // they are ints so that we can use switch on them
    int UseMInfApproximation = 1;
    int UseHInfApproximation = 1;

    double m_inf_cache[2000];
    double h_inf_cache[2000];
    double tau_m_cache[2000];
    double tau_h_cache[2000];

    // is this a calcium channel? 
    bool is_calcium = false;


    // default constructor
    conductance() {
        container = nullptr; //null pointer for safety
    }

    ~conductance() {}

    virtual void integrate(double, double);
    virtual void integrateMS(int, double, double);
    virtual void integrateLangevin(double, double);

    virtual void connect(compartment*); 
    virtual double getState(int);
    virtual double getCurrent(double);
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

    virtual void init(void);


    // fast random number generator
    static double gaussrand(void);


    // housekeeping, temp variables
    double minf = 0;
    double hinf = 1;
    double taum = 1;
    double tauh = 1;

    Permeability perm; 

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


    UseMInfApproximation = 0;
    UseHInfApproximation = 0;


    if (approx_channels == 0) {
        // turn off approximations
  
        UseMInfApproximation = 0;
        UseHInfApproximation = 0;


        if (verbosity==0) {
            mexPrintf("%s NOT using approximate activation functions because approx_channels is set to 0.\n", name.c_str());  
        }

        return;
    } else {
        // we should use approximations if possible
        if (AllowMInfApproximation) {UseMInfApproximation = 1;}
        if (AllowHInfApproximation) {UseHInfApproximation = 1;}

    }

    double V = 0;

    if (UseMInfApproximation == 1) {
        if (verbosity==0) {
            mexPrintf("%s using approximate activation functions\n", name.c_str());  
        }
    

        for (int V_int = -999; V_int < 1001; V_int++) {
            V = ((double) V_int)/10;
            m_inf_cache[V_int+999] = m_inf(V,0);
            tau_m_cache[V_int+999] = tau_m(V,0);
        }
    } else {
        if (verbosity==0) {
            mexPrintf("%s NOT USING approximate activation functions\n", name.c_str());  
        }
    }

    if (UseHInfApproximation == 1) {
        if (verbosity==0) {
           mexPrintf("%s using approximate in-activation functions\n", name.c_str()); 
        }
        
        for (int V_int = -999; V_int < 1001; V_int++) {
            V = ((double) V_int)/10;
            h_inf_cache[V_int+999] = h_inf(V,0);
            tau_h_cache[V_int+999] = tau_h(V,0);
        }
    } 
} // buildLUT




/*

This method is a dirty hack to speed up computing
exponents in C++. This requires that the power that a 
number is raised to be an integer (0-8)

*/
inline double conductance::fast_pow(double x, int a) {
    switch (a) {
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
        default: 
            return pow(x,a);
            break;
    }
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

This method returns the current that flows through
this channel at this moment. 

*/
double conductance::getCurrent(double V) { return g * (V - E); }



/*
*/
void conductance::checkSolvers(int solver_order) {
    if (solver_order == 0){
        return;
    } else if (solver_order == 4) {
        return;
    } else {
        mexPrintf("Error using %s", name.c_str());
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





/*
This method implements a very fast Gaussian random
number generator. This is much faster than the 
built-in generators in the C++ `<random>` header, and
is copied from Knuth and Marsaglia.

For the original source, see
"A Convenient Method for Generating Normal Variables"
SIAM Rev., 6(3), 260–264.
*/
double conductance::gaussrand() {
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if(phase == 0) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
            } while(S >= 1 || S == 0);

        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);

    phase = 1 - phase;

    return X;
}


/*
The virtual method returns the "state" of a conductance.
You can use this to read out whatever you want from
this conductance. 
*/

double conductance::getState(int i) {
    return 0;
}

