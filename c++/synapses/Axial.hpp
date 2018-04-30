// One way Axial synapse
// meant to be used for multi-compartment models
#ifndef AXIAL
#define AXIAL
#include "../synapse.hpp"

class Axial: public synapse {

public:

    double resistivity;

    // specify parameters + initial conditions 
    Axial(double resistivity_, double gbar_)
    {
        resistivity = resistivity_;
        gbar = gbar_;

        if (isnan(resistivity)) {resistivity = 1e-3;}
    }
    
    void integrate(double dt);
    void connect(compartment *pcomp1_, compartment *pcomp2_);
    double getCurrent(double V_post);
};

void Axial::integrate(double dt)
{       
    // set E to V_pre
    E = (pre_syn->V);
}

double Axial::getCurrent(double V_post)
{   
    return 0;

}

void Axial::connect(compartment *pcomp1_, compartment *pcomp2_) 
{
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // connect to the compartment 
    post_syn->addAxial(this);

    // calculate the gbar from the areas
    // of the two compartments using 
    // equation 6.30 (Dayan and Abbott)

    double Lmu = post_syn->len;
    double Lmup = pre_syn->len;
    double amu = post_syn->radius;
    double amup = pre_syn->radius;

    gbar = (amu*amup*amup);
    gbar = gbar/((resistivity*Lmu)*(Lmu*amup*amup + Lmup*amu*amu));
}

#endif
