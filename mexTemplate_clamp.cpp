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
    double *output_I_clamp;
    double *output_cond_state;

    // make an empty network 
    network STG;

    int nsteps;

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
    

    nsteps = (int) floor(tstop/dt); 
    int n_comp = (int) (STG.comp).size(); // these many compartments

    // compute cond_state_dim
    int cond_state_dims[n_comp];
    int cond_state_dim = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        int n_cond = ((STG.comp[i])->n_cond);
        cond_state_dims[i] = 2*n_cond;
        cond_state_dim += 2*n_cond;
    }

    plhs[0] = mxCreateDoubleMatrix(n_comp, nsteps, mxREAL); // V
    plhs[1] = mxCreateDoubleMatrix(n_comp, nsteps, mxREAL); // Ca
    plhs[2] = mxCreateDoubleMatrix(1, nsteps, mxREAL); // I_clamp
    plhs[3] = mxCreateDoubleMatrix(cond_state_dim, nsteps, mxREAL); // cond_state
    output_V = mxGetPr(plhs[0]);
    output_Ca = mxGetPr(plhs[1]);
    output_I_clamp = mxGetPr(plhs[2]);
    output_cond_state = mxGetPr(plhs[3]);

    double * full_cond_state = new double[cond_state_dim];
    int cond_idx = 0;

    
    for(int i = 0; i < nsteps; i++)
    {
        cond_idx = 0;
        STG.integrateClamp(V_drive[i],dt);
        output_I_clamp[i] = STG.comp[0]->I_clamp;
        for (int j = 0; j < n_comp; j++)
        {
            output_V[i*n_comp + j] = STG.comp[j]->V;
            output_Ca[i*n_comp + j] = STG.comp[j]->Ca;

            STG.comp[j]->get_cond_state(full_cond_state);
            
            for (int k = 0; k < cond_state_dims[j]; k++)
            {
                output_cond_state[i*cond_state_dim + cond_idx] = full_cond_state[k];
                cond_idx += 1;

            }


        }

    }
}
