// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Sensor of Calcium current 
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//

#pragma once
#include "mechanism.hpp"
#include <limits>


//inherit mechanism class spec
class FastSensor: public mechanism {

protected:
    double m_inf;
    double h_inf;
    double i_Ca;

    
public:

    // primary dynamical variables
    double X;
    double m = 0;
    double h = 1;

    // parameters for FastSensor
    double ZM;
    double ZH;
    double tau_m;
    double tau_h;
    double G;


    // specify parameters + initial conditions 
    FastSensor(double ZM_, double ZH_, double tau_m_, double tau_h_, double G_, double X_, double m_, double h_)
    {
        // wiring 
        ZM = ZM_;
        ZH = ZH_;
        tau_m = tau_m_;
        tau_h = tau_h_;
        G = G_;
        X = X_;


        fullStateSize = 1;

        // these defaults make it a "F" type sensor
        if (isnan(ZM)) {ZM = 14.2;}
        if (isnan(ZH)) {ZH = 9.8;}
        if (isnan(tau_m)) {tau_m = .5; } // ms
        if (isnan(tau_h)) {tau_h = 1.5;} // ms
        if (isnan(G)) {G = 10;}
        if (isnan(X)) {X = 0;}
        if (isnan(m)) {m = 0;}
        if (isnan(h)) {h = 1;}

        name = "FastSensor";


    }

    void integrate(void);


    void connectCompartment(compartment*);

    double boltzmann(double);


    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    

};

double FastSensor::getState(int idx){return X;}



int FastSensor::getFullState(double *cont_state, int idx) {
    cont_state[idx] = X;
    idx++;
    return idx;
}

// connection methods
void FastSensor::connectCompartment(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);
}



void FastSensor::integrate(void) {
    

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
double FastSensor::boltzmann(double x) {
    return 1/(1 + exp(x));
}





