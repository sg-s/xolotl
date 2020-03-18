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
    double* s_out;



    double* V_in  = mxGetPr(prhs[0]);
    double* params  = mxGetPr(prhs[1]);

    const mwSize* NN;
    NN = mxGetDimensions(prhs[0]);
    int N = NN[0];


    // mexPrintf("N=%i\n",N);
   


    // construct a dummy compartment 
    compartment PreSyn(0,.05,10,1,1,1,1,0,0,1,1,1,1);

    plhs[0] = mxCreateDoubleMatrix(N, 1, mxREAL);
    s_out = mxGetPr(plhs[0]);


    //xolotl:construct_synapse_here


    // connect synapse and configure
    ThisSyn.pre_syn = &PreSyn;
    ThisSyn.dt = .1;
    ThisSyn.s = 0;

    for (int i = 0; i < N; i++) {
        PreSyn.V = V_in[i];
        ThisSyn.integrate();
        s_out[i] = ThisSyn.s;
    }


}
