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
    double *output_state;
    double *output_V;
    double *output_Ca;
    double *output_I_clamp;
    double *output_cond_state; // conductances
    double *output_syn_state;  // synapses
    double *output_cont_state; // controllers


    //xolotl:define_v_clamp_idx

    // make an empty network
    network xolotl_network;

    int n_synapses = 0; // keeps track of how many synapses we have

    //xolotl:input_declarations


    double delta_temperature = (temperature - temperature_ref)/10;

    //xolotl:insert_constructors

    //xolotl:add_conductances_here

    //xolotl:add_synapses_here

    //xolotl:add_controllers_here

    //xolotl:add_neurons_to_network

    int nsteps = (int) floor(t_end/sim_dt);
    int nsteps_out = (int) floor(t_end/dt);
    int n_comp = (int) (xolotl_network.comp).size(); // these many compartments

    // compute cond_state_dim
    int cond_state_dims[n_comp];
    int cond_state_dim = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        int n_cond = ((xolotl_network.comp[i])->n_cond);
        cond_state_dims[i] = 2*n_cond; // only m, h -- gbar will be returned with controllers, if any
        cond_state_dim += 2*n_cond;
    }


    int res = dt/sim_dt;
    plhs[0] = mxCreateDoubleMatrix(1, param_size, mxREAL); //output state
    //xolotl:disable_for_NOCL_start
    plhs[1] = mxCreateDoubleMatrix(n_comp, nsteps_out, mxREAL); // V
    plhs[2] = mxCreateDoubleMatrix(2*n_comp, nsteps_out, mxREAL); // Ca + E_Ca
    plhs[3] = mxCreateDoubleMatrix(1, nsteps_out, mxREAL); // I_clamp
    plhs[4] = mxCreateDoubleMatrix(cond_state_dim, nsteps_out, mxREAL); // cond_state
    plhs[5] = mxCreateDoubleMatrix(2*n_synapses, nsteps_out, mxREAL); // synapse gbar + state
    plhs[6] = mxCreateDoubleMatrix(2*n_controllers, nsteps_out, mxREAL); // controllers gbar + mrna

    output_state = mxGetPr(plhs[0]);
    output_V = mxGetPr(plhs[1]);
    output_Ca = mxGetPr(plhs[2]);
    output_I_clamp = mxGetPr(plhs[3]);
    output_cond_state = mxGetPr(plhs[4]);
    output_syn_state = mxGetPr(plhs[5]);
    output_cont_state = mxGetPr(plhs[6]);

    // make arrays which will store the full cond. state
    double * full_cond_state = new double[cond_state_dim];
    int cond_idx = 0;


    // get the external currents
    double * I_ext  = mxGetPr(prhs[1]);

    // get the voltage clamps
    double * V_clamp  = mxGetPr(prhs[2]);


    // figure out if we're voltage clamping
    bool is_voltage_clamped = false;
    for (int j = 0; j < n_comp; j++)
    {
        if (!isnan(V_clamp[j]))
        {
            is_voltage_clamped = true;
        }
    }



    if (is_voltage_clamped)
    {
        // do the integration respecting V_clamp
        int output_idx = 0;
        for(int i = 0; i < nsteps; i++)
        {

            xolotl_network.integrateClamp(sim_dt, V_clamp, delta_temperature);

            //xolotl:read_synapses_here
            //xolotl:read_controllers_here

            // here we're getting the state of every compartment -- V, Ca, and all conductances
            if (i%res == 0)
            {
                output_idx ++;
                cond_idx = 0;
                for (int j = 0; j < n_comp; j++)
                {
                    if (isnan(V_clamp[j]))
                    {
                        output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
                    }
                    else {
                        output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->I_clamp;
                    }
                    
                    output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                    output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;

                    xolotl_network.comp[j]->get_cond_state(full_cond_state);

                    // get the states of every conductance
                    for (int k = 0; k < cond_state_dims[j]; k++)
                    {
                        output_cond_state[output_idx*cond_state_dim + cond_idx] = full_cond_state[k];
                        cond_idx ++;
                    }
                }
            }
        } // end for loop over nsteps
    }
    else 
        // voltage is not clamped
    {
        // do the integration
        int output_idx = 0;
        for(int i = 0; i < nsteps; i++)
        {
            //xolotl:disable_when_clamped
            xolotl_network.integrate(sim_dt,I_ext, delta_temperature);

            //xolotl:enable_when_clamped
            //xolotl_network.integrateClamp(sim_dt, V_clamp, delta_temperature);
            //xolotl:enable_when_clamped
            //output_Idx_clamp[i] = xolotl_network.comp[0]->I_clamp;

            //xolotl:read_synapses_here

            //xolotl:read_controllers_here

            // here we're getting the state of every compartment -- V, Ca, and all conductances
            if (i%res == 0)
            {
                output_idx ++;
                cond_idx = 0;
                for (int j = 0; j < n_comp; j++)
                {
                    output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;

                    output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                    output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;

                    xolotl_network.comp[j]->get_cond_state(full_cond_state);

                    // get the states of every conductance
                    for (int k = 0; k < cond_state_dims[j]; k++)
                    {
                        output_cond_state[output_idx*cond_state_dim + cond_idx] = full_cond_state[k];
                        cond_idx ++;
                    }
                }
            }
        } // end for loop over nsteps
    }




    // now measure the mean Ca in every compartment
    for(int j = 0; j < n_comp; j++)
    {
        (xolotl_network.comp[j]->Ca_average) = (xolotl_network.comp[j]->Ca_average)/nsteps;
    }

    // here, we are reading the full state back, so we 
    // can easily update the xolotl object in the MATLAB wrapper
    //xolotl:read_state_back


}
