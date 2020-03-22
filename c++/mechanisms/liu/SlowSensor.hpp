// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Sensor of Calcium current 
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//

#ifndef SLOWSENSOR
#define SLOWSENSOR
#include "mechanism.hpp"
#include <limits>


//inherit mechanism class spec
class SlowSensor: public mechanism {

protected:
    double m_inf;
    double h_inf;
    double i_Ca;

    
public:

    // primary dynamical variable s
    double X;
    double m = 0;
    double h = 1;

    // parameters for SlowSensor
    double ZM;
    double ZH;
    double tau_m;
    double tau_h;
    double G;

    // specify parameters + initial conditions 
    SlowSensor(double ZM_, double ZH_, double tau_m_, double tau_h_, double G_, double X_, double m_, double h_)
    {
        // wiring 
        ZM = ZM_;
        ZH = ZH_;
        tau_m = tau_m_;
        tau_h = tau_h_;
        G = G_;
        X = X_;

        // these defaults make it a "S" type sensor
        if (isnan(ZM)) {ZM = 7.2;}
        if (isnan(ZH)) {ZH = 2.8;}
        if (isnan(tau_m)) {tau_m = 50; } // ms
        if (isnan(tau_h)) {tau_h = 60;} // ms
        if (isnan(G)) {G = 3;}
        if (isnan(X)) {X = 0;}
        if (isnan(m)) {m = 0;}
        if (isnan(h)) {h = 1;}


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

string SlowSensor::getClass() {
    return "SlowSensor";
}


double SlowSensor::getState(int idx){return X;}


int SlowSensor::getFullStateSize(){return 1; }


int SlowSensor::getFullState(double *cont_state, int idx) {
    cont_state[idx] = X;
    idx++;
    return idx;
}

// connection methods
void SlowSensor::connect(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}

void SlowSensor::connect(conductance* cond_) {
    mexErrMsgTxt("[SlowSensor] This mechanism cannot connect to a conductance object");
}

void SlowSensor::connect(synapse* syn_) {
    mexErrMsgTxt("[SlowSensor] This mechanism cannot connect to a synapse object");
}



void SlowSensor::integrate(void) {
    

    // convert i_Ca into nA/nF
    i_Ca = (comp->i_Ca_prev)/(comp->Cm);

    // compute minf, hinf
    m_inf = boltzmann(ZM + i_Ca);
    h_inf = boltzmann(-ZH - i_Ca);

    // compute m and h
    m = m_inf + (m - m_inf)*exp(-dt/tau_m);
    h = h_inf + (h - h_inf)*exp(-dt/tau_h);

    X = G*m*m*h;

}

// activation/inactivation functions
double SlowSensor::boltzmann(double x) {
    return 1/(1 + exp(x));
}



void SlowSensor::checkSolvers(int k) {
    if (k == 0) {return;}
    else {mexErrMsgTxt("[SlowSensor] unsupported solver order\n");}
}



#endif
