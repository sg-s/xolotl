// this wrapper tests to see if voltage clamping
// and electrical synapses 
// works as intended 

#include <cmath>
#include <vector>
#include <typeinfo>
#include "mex.h"
#include "../../compartment.h"
#include "../../network.h"

// use the Liu conductances
#include "../../conductances/liu/NaV.h"
#include "../../conductances/liu/Kd.h"
#include "../../conductances/Leak.h"

#include "../../conductances/Electrical.h"

using namespace std;

// usage [output] = f([dt tstop res],[gbars], [inital_conditions])
// 1 -- voltage
// 2 -- calcium concentration 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    double *output_V;
    double *output_Ca;
    double *output_I_clamp;
    double *V_clamp;
    // double *simparams, *gmax, *initial_conditions, *neuron_params;
    int nits = 10;
    double dt, tstop;
    dt  = 50e-3;
    tstop = 5000;
    // default conductances
    double gbar_na = 9930.0, gbar_kd = 1610.0, gbar_h = 10.1, gbar_leak = .99;
    // default reversal potentials
    double e_leak = -50.0, e_na = 30.0, e_k = -80.0, e_h = -20.0;
    // default initial conditions
    double V0 = -60.0, Ca0 = 0.05, mNa = 0, mCaT = 0, mCaS = 0, mA = 0, mKCa = 0 , mKd = 0, mH = 0, hNa = 0, hCaT = 0, hCaS = 0, hA = 0;
    // default parameters
    double Ca_in = 0.05, Ca_out = 3000.0, tau_Ca = 200.0, Cm = 10.0, A = .0628, f = 1.496;


    
    // parse inputs
    // prhs contains the arguments in the MATLAB wrapper

    V_clamp = mxGetPr(prhs[0]);

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

    
    // make conductances
    Leak gleak(gbar_leak,e_leak);
    NaV gna(gbar_na,e_na, mNa, hNa);
    Kd gkd(gbar_kd,e_k, mKd);
    
    // make compartment and add all the conductances
    compartment cell(V0, Ca0, Cm, A, f, Ca_out, Ca_in, tau_Ca);
    cell.addConductance(&gna);
    cell.addConductance(&gkd);
    cell.addConductance(&gleak);

    compartment cell_clamped(V0, Ca0, Cm, A, f, Ca_out, Ca_in, tau_Ca);

    // wire them up
    Electrical g1(10e3); g1.connect(&cell,&cell_clamped);
    Electrical g2(10e3); g2.connect(&cell_clamped,&cell);

    network N;
    N.addCompartment(&cell_clamped);
    N.addCompartment(&cell);
    


    nits = (int) floor(tstop/dt);
    int n_comp = (int) (N.comp).size(); // these many compartments

    plhs[0] = mxCreateDoubleMatrix(n_comp, nits, mxREAL); // V
    plhs[1] = mxCreateDoubleMatrix(n_comp, nits, mxREAL); // Ca
    plhs[2] = mxCreateDoubleMatrix(n_comp, nits, mxREAL); // I_clamp
    output_V = mxGetPr(plhs[0]);
    output_Ca = mxGetPr(plhs[1]);
    output_I_clamp = mxGetPr(plhs[2]);

    // mexPrintf("size of output_V: %d\n",n_comp);

    for(int i = 0; i < nits; i++)
    {
         N.integrateClamp(V_clamp[i], dt);

        for (int j = 0; j < n_comp; j++)
        {
            output_V[i*n_comp + j] = N.comp[j]->V;
            output_Ca[i*n_comp + j] = N.comp[j]->Ca;
            output_I_clamp[i*n_comp + j] = N.comp[j]->I_clamp;
        }

    }
}
