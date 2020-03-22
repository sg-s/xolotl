// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Sensor of Calcium current 
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//

#ifndef DCSENSOR
#define DCSENSOR
#include "mechanism.hpp"
#include <limits>


//inherit mechanism class spec
class DCSensor: public mechanism {

protected:
    double m_inf;
    double h_inf;
    double i_Ca;

    
public:

    // primary dynamical variable s
    double X;
    double m = 0;
    double h = 1;

    // parameters for DCSensor
    double ZM;
    double tau_m;
    double G;

    // specify parameters + initial conditions 
    DCSensor(double ZM_, double tau_m_, double G_, double X_, double m_)
    {
        // wiring 
        ZM = ZM_;
        tau_m = tau_m_;
        G = G_;
        X = X_;


        // these defaults make it a "F" type sensor
        if (isnan(ZM)) {ZM = 3;}
        if (isnan(tau_m)) {tau_m = 500; } // ms
        if (isnan(G)) {G = 1;}
        if (isnan(X)) {X = 0;}
        if (isnan(m)) {m = 0;}


    }

    void checkSolvers(int);

    void integrate(void);


    void connect(compartment*);
    void connect(conductance*);
    void connect(synapse*);

    double boltzmann(double);


    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

};

string DCSensor::getClass() {
    return "DCSensor";
}


double DCSensor::getState(int idx){return X;}


int DCSensor::getFullStateSize(){return 1; }


int DCSensor::getFullState(double *cont_state, int idx) {
    cont_state[idx] = X;
    idx++;
    return idx;
}

// connection methods
void DCSensor::connect(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}

void DCSensor::connect(conductance* cond_) {
    mexErrMsgTxt("[DCSensor] This mechanism cannot connect to a conductance object");
}

void DCSensor::connect(synapse* syn_) {
    mexErrMsgTxt("[DCSensor] This mechanism cannot connect to a synapse object");
}



void DCSensor::integrate(void) {
    

    // convert i_Ca into nA/nF
    i_Ca = (comp->i_Ca_prev)/(comp->Cm);

    // compute minf
    m_inf = boltzmann(ZM + i_Ca);

    // compute m and h
    m = m_inf + (m - m_inf)*exp(-dt/tau_m);


    X = G*m*m;

}

// activation/inactivation functions
double DCSensor::boltzmann(double x) {
    return 1/(1 + exp(x));
}



void DCSensor::checkSolvers(int k) {
    if (k == 0) {return;}
    else {mexErrMsgTxt("[DCSensor] unsupported solver order\n");}
}



#endif
