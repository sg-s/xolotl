// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// FSDTarget
// implements a target mechanism for the FSD controller
// as in Liu et al. J Neuro
// note that the "F" variable in the paper
// is represented here by FF

#ifndef FSDTARGET
#define FSDTARGET
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class FSDTarget: public mechanism {

protected:

    // housekeeping variable
    double xinf = 0;
    double norm_i_Ca = 0;

    double EMF = 0;
    double EMS = 0;
    double EMD = 0;
    double EHF = 0;
    double EHS = 0;


public:
    // timescales
    double Fbar = std::numeric_limits<double>::quiet_NaN();
    double Sbar = std::numeric_limits<double>::quiet_NaN();
    double Dbar = std::numeric_limits<double>::quiet_NaN();


    // dynamic variables
    double FF = 0;
    double S = 0;
    double D = 0;

    double MF = 0;
    double HF = 0;
    double MS = 0;
    double HS = 0;
    double MD = 0;


    double GF = 10;
    double GS = 3;
    double GD = 1;

    // Z parameters, as in Table 2 of Liu et al.
    double ZM_F = 14.2;
    double ZM_S = 7.2;
    double ZM_D = 3;

    double ZH_F = 9.8;
    double ZH_S = 2.8;


    // timescales
    double tau_MF = .5; // ms
    double tau_MS = 50;
    double tau_MD = 500;

    double tau_HF = 1.5;
    double tau_HS = 60;


    // specify parameters + initial conditions for
    // mechanism that controls a conductance
    FSDTarget(double Fbar_, double Sbar_, double Dbar_, double FF_, double S_, double D_, double MF_, double HF_, double MS_, double HS_, double MD_)
    {


        MF = MF_;
        HF = HF_;
        MS = MS_;
        HS = HS_;
        MD = MD_;


        FF = FF_;
        S = S_;
        D = D_;

        if (isnan(FF)) {FF = 0;}
        if (isnan(S)) {S = 0;}
        if (isnan(D)) {D = 0;}

        if (isnan(MF)) {MF = 0;}
        if (isnan(HF)) {HF = 0;}
        if (isnan(MS)) {MS = 0;}
        if (isnan(HS)) {HS = 0;}
        if (isnan(MD)) {MD = 0;}


        Fbar = Fbar_;
        Sbar = Sbar_;
        Dbar = Dbar_;

        // abuse the controlling_class property
        // so that FSDController can find this mechanism
        controlling_class = "FSD";

    }


    void integrate(void);

    double MH_inf(double, double);


    void checkSolvers(int);

    void connect(conductance *);
    void connect(synapse*);
    void connect(compartment*);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

};

string FSDTarget::getClass() {
    return "FSDTarget";
}

// it has no state since this is only a container for
// the target
double FSDTarget::getState(int idx){
    switch (idx) {
        case 0:
            return Fbar;
        case 1:
            return Sbar;
        case 2:
            return Dbar;
        case 3:
            return FF;
        case 4:
            return S;
        case 5:
            return D;
        default:
            return std::numeric_limits<double>::quiet_NaN();
    }

}

// return F, S D
int FSDTarget::getFullStateSize(){return 3; }


int FSDTarget::getFullState(double * mech_state, int idx){
    // return F, S, D and MF, HF
    mech_state[idx] = FF; idx++;
    mech_state[idx] = S; idx++;
    mech_state[idx] = D; idx++;

    return idx;
}


void FSDTarget::connect(conductance * channel_){

    mexErrMsgTxt("[FSDTarget] This mechanism cannot connect to a conductance object");

}

void FSDTarget::connect(compartment* comp_) {
    comp = comp_;
    comp->addMechanism(this);

    // preallocate something to speed up computation
    EMF = exp(-dt/tau_MF);
    EMS = exp(-dt/tau_MS);
    EMD = exp(-dt/tau_MD);
    EHF = exp(-dt/tau_HF);
    EHS = exp(-dt/tau_HS);

}

void FSDTarget::connect(synapse* syn_) {
    // connect to a synpase
    mexErrMsgTxt("[FSDTarget] This mechanism cannot connect to a synapse object");

}

// there is nothing to do since this mechanism has no state
void FSDTarget::integrate(void){

    // integrate M and H variables

    norm_i_Ca = comp->i_Ca_prev/comp->Cm;


    // MF
    xinf = MH_inf(norm_i_Ca, ZM_F); // now in units of nA/nF
    MF = xinf + (MF - xinf)*EMF;

    // MS
    xinf = MH_inf(norm_i_Ca, ZM_S); // now in units of nA/nF
    MS = xinf + (MS - xinf)*EMS;

    // MD
    xinf = MH_inf(norm_i_Ca, ZM_D); // now in units of nA/nF
    MD = xinf + (MD - xinf)*EMD;

    // HF
    xinf = MH_inf(-norm_i_Ca, -ZH_F); // now in units of nA/nF
    HF = xinf + (HF - xinf)*EHF;

    // HS
    xinf = MH_inf(-norm_i_Ca, -ZH_S); // now in units of nA/nF
    HS = xinf + (HS - xinf)*EHS;

    // now update the FSD variables
    FF = GF*MF*MF*HF;
    S = GS*MS*MS*HS;
    D = GD*MD*MD;

}

// exponential functions for gating like variables
double FSDTarget::MH_inf(double I_Ca, double Z) {
    return 1/(1 + exp(Z + I_Ca));
}


void FSDTarget::checkSolvers(int k) {
    if (k == 0){
        return;
    } else {
        mexErrMsgTxt("[IntegralController] unsupported solver order\n");
    }
}





#endif
