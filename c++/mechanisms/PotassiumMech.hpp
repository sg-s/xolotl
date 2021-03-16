// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component source [O'Leary et al. 2014](https://www.sciencedirect.com/science/article/pii/S089662731400292X)
// component info: Integral controller of conductances and synapses

#ifndef POTASSIUMMECH
#define POTASSIUMMECH
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class PotassiumMech: public mechanism {

protected:

    

    // we use a vector to keep pointers to mechanisms that 
    // contribute to RHS terms in the mRNA ODE
    vector<conductance*> K_channels;


    double RT_by_nF = 0;

public:


    double K_in = 150e-6;
    double K_out = 5.5e-6;

    // total potassium current through all channels
    double i_K = 0;

    double E_K = -80;


    // buffering time for potassium assumed to be very slow
    double tau_K = 1e9; 

    // pump params
    double i_max = 1e5;
    double K_D = 5.5e-6;
    double Delta = -1.1e-6;


    double i_pump = 0;


    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    PotassiumMech(double K_in_, double K_out_, double i_K_, double E_K_, double tau_K_, double i_max_, double K_D_, double Delta_) {

        K_in = K_in_;
        K_out = K_out_;
        i_K = i_K_;
        E_K = E_K_;
        tau_K = tau_K_;

        i_max = i_max_;
        K_D = K_D_;
        Delta = Delta_;

        fullStateSize = 4;


        name = "PotassiumMech";
        mechanism_type = "ion";
    }



    // fcn declarations
    void integrate(void);
    void init(void);
    double getState(int);
    

};


void PotassiumMech::init() {


    // connect to every potassium channel in the compartment
    for (int i = 0; i < comp->n_cond; i ++) {
        if ((comp->getConductancePointer(i))->perm.K == 1) {
            K_channels.push_back(comp->getConductancePointer(i));
        }
    }



    if (verbosity == 0) {
        mexPrintf("Connected to %i potassium channels\n", K_channels.size());
    }
    

    RT_by_nF = .086169*(temperature+273.15);

}


double PotassiumMech::getState(int idx) {
    switch (idx) {
        case 0:
            return E_K;
            break;
        case 1:
            return i_K;
            break;
        case 2:
            return i_pump;
            break;
        case 3:
            return K_in;
            break;
        default:
            return std::numeric_limits<double>::quiet_NaN();
    }

}



void PotassiumMech::integrate(void) {


    i_K = 0;

    // read out all potassium currents and update
    for (int i = 0; i < K_channels.size(); i++) {
        i_K += K_channels.at(i)->getCurrent(comp->V_prev);
    }


    // compute pump current
    i_pump = i_max/(1 + exp((K_out - K_D)/(Delta)));




    // convert net potassium current into a change in the
    // potassium conc
    K_in += (-i_K + i_pump)*(dt/tau_K);


    if (K_in<0) {K_in = 0;}


    // compute E_K
    E_K = RT_by_nF*(log(K_out/K_in));

    // update E of potassium channels
    for (int i = 0; i < K_channels.size(); i++) {
        K_channels.at(i)->E = E_K;
    }

}




#endif
