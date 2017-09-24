#include <cmath>
#include <vector>
#include <typeinfo>
#include "mex.h"

//xolotl:include_headers_here


using namespace std;

 
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    double *output_V;
    double *output_Ca;
    double *I_clamp;

    // make an empty network 
    network STG;

    int nits;

    // wire up simulation parameters
    double * simparams = mxGetPr(prhs[0]);

    double dt  = simparams[0];
    double tstop = simparams[1];

    int v_drive_idx;
    //xolotl:define_v_drive_idx
    double *V_drive = mxGetPr(prhs[v_drive_idx]);

    //xolotl:input_declarations 

    //xolotl:make_compartments_here 

    //xolotl:make_conductances_here
    
    //xolotl:add_conductances_here

    //xolotl:add_synapses_here

    //xolotl:add_neurons_to_network
    
    nits = (int) floor(tstop/dt); 
    int n_comp = (int) (STG.comp).size(); // these many compartments

    plhs[0] = mxCreateDoubleMatrix(n_comp, nits, mxREAL); // V
    plhs[1] = mxCreateDoubleMatrix(n_comp, nits, mxREAL); // Ca
    plhs[2] = mxCreateDoubleMatrix(1, nits, mxREAL); // I_clamp
    output_V = mxGetPr(plhs[0]);
    output_Ca = mxGetPr(plhs[1]);
    I_clamp = mxGetPr(plhs[2]);
    
    for(int i = 0; i < nits; i++)
    {
        STG.integrateClamp(V_drive[i],dt);
        I_clamp[i] = STG.comp[0]->I_clamp;
        for (int j = 0; j < n_comp; j++)
        {
            output_V[i*n_comp + j] = STG.comp[j]->V;
            output_Ca[i*n_comp + j] = STG.comp[j]->Ca;
        }

    }
}
