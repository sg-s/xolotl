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
    double *minf;
    double *hinf;
    double *taum;
    double *tauh;


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
    minf = mxGetPr(plhs[0]);

    plhs[1] = mxCreateDoubleMatrix(N, 1, mxREAL);
    hinf = mxGetPr(plhs[1]);

    plhs[2] = mxCreateDoubleMatrix(N, 1, mxREAL);
    taum = mxGetPr(plhs[2]);

    plhs[3] = mxCreateDoubleMatrix(N, 1, mxREAL);
    tauh = mxGetPr(plhs[3]);

    // V  Ca  Cm  A  vol    Ca_average  tree_idx  neuron_idx  radius  len  shell_thickness  Ca_out_)
    compartment comp(0, Ca_in, 10, 1, 1, .05, 0, 0, 1, 1, 1, Ca_out);

    //xolotl:construct_channel_here

    comp.temperature = temperature;
    comp.temperature_ref = temperature_ref;

    comp.addConductance(&ThisChannel);

    comp.init();

    

    for (int i = 0; i < N; i ++) {
        minf[i] = ThisChannel.m_inf(Vspace[i],Ca_in);
        hinf[i] = ThisChannel.h_inf(Vspace[i],Ca_in);
        taum[i] = ThisChannel.tau_m(Vspace[i],Ca_in);
        tauh[i] = ThisChannel.tau_h(Vspace[i],Ca_in);
    }


}
