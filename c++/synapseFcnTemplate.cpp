// xolotl
// this template file is used by xolotl.compileActivationFcn() to convert
// the xolotl pseudo-object in MATLAB into a C++
// file that can be compiled from mex
// DON'T TOUCH ANYTHING HERE!

#include <cmath>
#include <vector>
#include <typeinfo>
#include "mex.h"


//xolotl:include_headers_here



using namespace std;


void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    // declare pointers to outputs
    double *sinf;
    double *taus;


    double * Vspace  = mxGetPr(prhs[0]);
    const mwSize *NN;
    NN = mxGetDimensions(prhs[0]);
    int N1 = NN[0];
    int N2 = NN[1];

    int N = 1;

    if (N1 > N2) {
        N = N1;
    } else {
        N = N2;
    }

    double * params  = mxGetPr(prhs[1]);


    double * coreParams  = mxGetPr(prhs[2]);
    double temperature_ref = coreParams[0];
    double temperature = coreParams[1];
    double Ca_in = coreParams[2];
    double Ca_out = coreParams[3];

    plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);
    sinf = mxGetPr(plhs[0]);

    plhs[1] = mxCreateDoubleMatrix(N, 1, mxREAL);
    taus = mxGetPr(plhs[1]);

    // V  Ca  Cm  A  vol    Ca_average  tree_idx  neuron_idx  radius  len  shell_thickness  Ca_out_)
    compartment PreSyn(0, Ca_in, 10, 1, 1, .05, 0, 0, 1, 1, 1, Ca_out);

    compartment PostSyn(0, Ca_in, 10, 1, 1, .05, 0, 0, 1, 1, 1, Ca_out);

    //xolotl:construct_synapse_here

    PreSyn.temperature = temperature;
    PreSyn.temperature_ref = temperature_ref;

    PostSyn.temperature = temperature;
    PostSyn.temperature_ref = temperature_ref;

    ThisSyn.connect(&PreSyn, &PostSyn);

    PostSyn.init();

    

    for (int i = 0; i < N; i ++) {
        sinf[i] = ThisSyn.s_inf(Vspace[i]);
        taus[i] = ThisSyn.tau_s(Vspace[i]);
    }


}
