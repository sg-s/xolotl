// One way Axial synapse
// meant to be used for multi-compartment models
#ifndef AXIAL
#define AXIAL

class Axial: public synapse {

public:

    double resistivity;

    // specify parameters + initial conditions 
    Axial(double resistivity_, double gmax_)
    {
        resistivity = resistivity_;
        gmax = gmax_;

        if (isnan(resistivity)) {resistivity = 1e-3;}

        if (resistivity == 0) {
            mexErrMsgIdAndTxt("xolotl:Axial", "resistivity cannot be zero.");
        }
    }
    
    void integrate(void);
    void connect(compartment *pcomp1_, compartment *pcomp2_);


};

void Axial::integrate(void) {       
    // set E to V_pre
    E = (pre_syn->V);
}


void Axial::connect(compartment *pcomp1_, compartment *pcomp2_) {
    pre_syn = pcomp1_; 
    post_syn = pcomp2_; 

    // connect to the compartment 
    post_syn->addAxial(this);

    // calculate the gmax from the areas
    // of the two compartments using 
    // equation 6.30 (Dayan and Abbott)

    double Lmu_ = post_syn->len;
    double amu_ = post_syn->radius;

    double amu = pre_syn->radius;
    double Lmu = pre_syn->len;

    gmax = (amu*amu_*amu_)/((resistivity*Lmu)*(Lmu*amu_*amu_ + Lmu_*amu*amu));

    if (isnan(gmax)) {
        mexPrintf("[WARNING] the maximal conductance of this axial synapse could not be computed. This is probably because you have not specified the geometrical parameters of all compartments.")
    }
}

#endif
