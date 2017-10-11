// xolotl
// this template file is used by xolotl.transpile() to convert
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
    double *output_V;
    double *output_Ca;
    double *output_I_clamp;
    double *output_cond_state; // conductances
    double *output_syn_state;  // synapses
    double *output_cont_state; // controllers 


    //xolotl:define_v_clamp_idx

    // make an empty network 
    network STG;

    int nsteps;
    int n_synapses = 0; // keeps track of how many synapses we have 

    // wire up simulation parameters
    double * simparams = mxGetPr(prhs[0]);

    double dt  = simparams[0];
    double tstop = simparams[1];

    //xolotl:input_declarations 

    //xolotl:make_compartments_here 

    //xolotl:make_conductances_here
    
    //xolotl:add_conductances_here

    //xolotl:add_synapses_here

    //xolotl:add_controllers_here

    //xolotl:add_neurons_to_network
    
    nsteps = (int) floor(tstop/dt); 
    int n_comp = (int) (STG.comp).size(); // these many compartments

    // compute cond_state_dim
    int cond_state_dims[n_comp];
    int cond_state_dim = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        int n_cond = ((STG.comp[i])->n_cond);
        cond_state_dims[i] = 2*n_cond; // only m, h -- gbar will be returned with controllers, if any
        cond_state_dim += 2*n_cond;
    }

    plhs[0] = mxCreateDoubleMatrix(n_comp, nsteps, mxREAL); // V
    plhs[1] = mxCreateDoubleMatrix(2*n_comp, nsteps, mxREAL); // Ca + E_Ca
    plhs[2] = mxCreateDoubleMatrix(1, nsteps, mxREAL); // I_clamp
    plhs[3] = mxCreateDoubleMatrix(cond_state_dim, nsteps, mxREAL); // cond_state
    plhs[4] = mxCreateDoubleMatrix(2*n_synapses, nsteps, mxREAL); // synapse gbar + state
    plhs[5] = mxCreateDoubleMatrix(2*n_controllers, nsteps, mxREAL); // controllers gbar + mrna

    output_V = mxGetPr(plhs[0]);
    output_Ca = mxGetPr(plhs[1]);
    output_I_clamp = mxGetPr(plhs[2]);
    output_cond_state = mxGetPr(plhs[3]);
    output_syn_state = mxGetPr(plhs[4]);
    output_cont_state = mxGetPr(plhs[5]);

    // make arrays which will store the full cond. state 
    double * full_cond_state = new double[cond_state_dim];
    int cond_idx = 0;

    
    for(int i = 0; i < nsteps; i++)
    {
        STG.integrate(dt);
        //xolotl:enable_when_clamped
        //STG.integrateClamp(V_clamp[i],dt); 
        //xolotl:enable_when_clamped
        //output_I_clamp[i] = STG.comp[0]->I_clamp; 

        //xolotl:read_synapses_here

        //xolotl:read_controllers_here

        // here we're getting the state of every compartment -- V, Ca, and all conductances
        cond_idx = 0;
        for (int j = 0; j < n_comp; j++)
        {
            output_V[i*n_comp + j] = STG.comp[j]->V;
            output_Ca[i*2*n_comp + j] = STG.comp[j]->Ca;
            output_Ca[i*2*n_comp + j + n_comp] = STG.comp[j]->E_Ca;

            STG.comp[j]->get_cond_state(full_cond_state);
            
            // get the states of every conductance 
            for (int k = 0; k < cond_state_dims[j]; k++)
            {
                output_cond_state[i*cond_state_dim + cond_idx] = full_cond_state[k];
                cond_idx ++;
            }
        }

    }
}
