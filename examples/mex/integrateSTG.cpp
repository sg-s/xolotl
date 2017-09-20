// integrateSTG.cpp
// integrates a 3-compartment STG model

#include <cmath>
#include <vector>
#include <typeinfo>
#include "mex.h"
#include "../../network.h"
#include "../../compartment.h"
#include "../../synapse.h"

// use Prinz conductances
#include "../../conductances/prinz/NaV.h"
#include "../../conductances/prinz/CaT.h"
#include "../../conductances/prinz/CaS.h"
#include "../../conductances/prinz/ACurrent.h"
#include "../../conductances/prinz/KCa.h"
#include "../../conductances/prinz/Kd.h"
#include "../../conductances/prinz/HCurrent.h"
#include "../../conductances/Leak.h"

// use Prinz synapses 
#include "../../conductances/prinz/Cholinergic.h"
#include "../../conductances/prinz/Glutamatergic.h"

using namespace std;

// usage [output] = f([dt tstop res],[gbars], [inital_conditions])
// 1 -- voltage
// 2 -- calcium concentration 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    double *output_V;
    double *output_Ca;
    // double *simparams, *gmax, *initial_conditions, *neuron_params;
    int nits = 10;
    double dt, tstop;
    dt  = 50e-3;
    tstop = 5000;
    // default conductances
    double gbar_na = 1830.0, gbar_cat = 23.0, gbar_cas = 27.0, gbar_a = 246.0, gbar_kca = 980.0, gbar_kd = 610.0, gbar_h = 10.1, gbar_leak = .99;
    // default reversal potentials
    double e_leak = -50.0, e_na = 50.0, e_k = -80.0, e_h = -20.0;
    // default initial conditions
    double V0 = -70.0, Ca0 = 0.05, mNa = 0, mCaT = 0, mCaS = 0, mA = 0, mKCa = 0 , mKd = 0, mH = 0, hNa = 0, hCaT = 0, hCaS = 0, hA = 0;
    // default parameters
    double Ca_in = 0.05, Ca_out = 3000.0, tau_Ca = 200.0, Cm = 10.0, A = .0628, f = 14.96;


    
    // // parse inputs
    // // prhs contains the arguments in the MATLAB wrapper
    // simparams = mxGetPr(prhs[0]);
    // gmax = mxGetPr(prhs[1]);
    // initial_conditions = mxGetPr(prhs[2]);
    // neuron_params = mxGetPr(prhs[3]);

    // // simulation parameters
    // dt = simparams[0];
    // tstop = simparams[1];
    // res = simparams[2];

    // // initial conditions
    // mNa = initial_conditions[0];
    // mCaT = initial_conditions[1];
    // mCaS = initial_conditions[2];
    // mA = initial_conditions[3];
    // mKCa = initial_conditions[4];
    // mKd= initial_conditions[5];
    // mH = initial_conditions[6];

    // hNa = initial_conditions[7];
    // hCaT = initial_conditions[8];
    // hCaS = initial_conditions[9];
    // hA = initial_conditions[10];

    // v0 = initial_conditions[11];
    // Ca0 = initial_conditions[12];


    // conductances
    // gbar_na = gmax[0];
    // gbar_cat = gmax[1];
    // gbar_cas = gmax[2];
    // gbar_ka = gmax[3];
    // gbar_kca = gmax[4];
    // gbar_kd = gmax[5];
    // gbar_h = gmax[6];
    // gbar_leak = gmax[7];

    // // neuron parameters
    // // neuron_parameters = [p.C p.A p.f p.extCa p.Ca0 p.tau_Ca e_Na e_K e_H e_leak];
    // C = neuron_params[0];
    // A = neuron_params[1];
    // f = neuron_params[2];
    // extCa = neuron_params[3];
    // Ca_inside_cell = neuron_params[4];
    // tau_Ca = neuron_params[5];


    // e_na = neuron_params[6];
    // e_k = neuron_params[7];
    // e_h = neuron_params[8];
    // e_leak = neuron_params[9];


    // make an empty network 
    network STG;
    
    // make conductances
    NaV gna_AB(1000,e_na, mNa, hNa);
    CaT gcat_AB(25,120.0, mCaT ,hCaT); 
    CaS gcas_AB(60,120.0, mCaS, hCaS); 
    ACurrent gka_AB(500,e_k, mA ,hA);
    KCa gkca_AB(50,e_k, mKCa);
    Kd gkd_AB(1000,e_k, mKd);
    HCurrent gh_AB(.1,e_h, mH);
    Leak gleak_AB(0,e_leak);
    
    // make AB and add all the conductances
    compartment AB(-70, .01, Cm, A, f, Ca_out, Ca_in, tau_Ca);
    AB.addConductance(&gna_AB);
    AB.addConductance(&gcat_AB);
    AB.addConductance(&gcas_AB);
    AB.addConductance(&gka_AB);
    AB.addConductance(&gkca_AB);
    AB.addConductance(&gkd_AB);
    AB.addConductance(&gh_AB);
    AB.addConductance(&gleak_AB);

    // make conductances for LP
    NaV gna_LP(1000,e_na, mNa, hNa);
    CaT gcat_LP(0,120.0, mCaT ,hCaT); 
    CaS gcas_LP(40,120.0, mCaS, hCaS); 
    ACurrent gka_LP(200,e_k, mA ,hA);
    KCa gkca_LP(0,e_k, mKCa);
    Kd gkd_LP(250,e_k, mKd);
    HCurrent gh_LP(.5,e_h, mH);
    Leak gleak_LP(.3,e_leak);

    // make LP neuron
    compartment LP(-70, .01, Cm, A, f, Ca_out, Ca_in, tau_Ca);
    LP.addConductance(&gna_LP);
    LP.addConductance(&gcat_LP);
    LP.addConductance(&gcas_LP);
    LP.addConductance(&gka_LP);
    LP.addConductance(&gkca_LP);
    LP.addConductance(&gkd_LP);
    LP.addConductance(&gh_LP);
    LP.addConductance(&gleak_LP);

    // make conductances for PY
    NaV gna_PY(1000,e_na, .04, .1);
    CaT gcat_PY(25,120.0, .27 ,.14); 
    CaS gcas_PY(20,120.0, .23, .07); 
    ACurrent gka_PY(500,e_k, .0018 ,.2);
    KCa gkca_PY(0,e_k, .8);
    Kd gkd_PY(1250,e_k, .06);
    HCurrent gh_PY(.5,e_h, .06);
    Leak gleak_PY(.1,e_leak);

    // make PY neuron 
    compartment PY(-70, .03, Cm, A, f, Ca_out, Ca_in, tau_Ca);
    PY.addConductance(&gna_PY);
    PY.addConductance(&gcat_PY);
    PY.addConductance(&gcas_PY);
    PY.addConductance(&gka_PY);
    PY.addConductance(&gkca_PY);
    PY.addConductance(&gkd_PY);
    PY.addConductance(&gh_PY);
    PY.addConductance(&gleak_PY);

    // make some synapses 
    Cholinergic PD2LP(30.0); PD2LP.connect(&AB,&LP);
    Cholinergic PD2PY(3.0); PD2PY.connect(&AB,&PY);
    Glutamatergic AB2LP(30.0); AB2LP.connect(&AB,&LP);
    Glutamatergic AB2PY(10.0); AB2PY.connect(&AB,&PY);
    Glutamatergic LP2PY(1.0); LP2PY.connect(&LP,&PY);
    Glutamatergic PY2LP(30.0); PY2LP.connect(&PY,&LP);
    Glutamatergic LP2AB(30.0); LP2AB.connect(&LP,&AB);

    // add neurons to the network
    STG.addCompartment(&AB);
    STG.addCompartment(&LP);
    STG.addCompartment(&PY);

    nits = (int) floor(tstop/dt);
    int n_comp = (int) (STG.comp).size(); // these many compartments

    plhs[0] = mxCreateDoubleMatrix(n_comp, nits, mxREAL); // V
    plhs[1] = mxCreateDoubleMatrix(n_comp, nits, mxREAL); // Ca
    output_V = mxGetPr(plhs[0]);
    output_Ca = mxGetPr(plhs[1]);

    // mexPrintf("size of output_V: %d\n",n_comp);

    for(int i = 0; i < nits; i++)
    {
        STG.integrate(dt);
        for (int j = 0; j < n_comp; j++)
        {
            output_V[i*n_comp + j] = STG.comp[j]->V;
            output_Ca[i*n_comp + j] = STG.comp[j]->Ca;
        }

    }
}
