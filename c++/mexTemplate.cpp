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
    double *output_curr_state; // currents
    double *output_syn_state;  // synapses
    double *output_cont_state; // controllers


    //xolotl:define_v_clamp_idx

    // make an empty network
    network xolotl_network;

    int n_synapses = 0;

    //xolotl:input_declarations


    double delta_temperature = (temperature - temperature_ref)/10;

    //xolotl:insert_constructors

    //xolotl:add_conductances_here

    //xolotl:add_synapses_here

    //xolotl:add_controllers_here

    //xolotl:add_neurons_to_network

    //xolotl:call_methods_here

    int nsteps = (int) floor(t_end/sim_dt);
    int nsteps_out = (int) floor(t_end/dt);
    int n_comp = (int) (xolotl_network.comp).size(); // these many compartments


    // ask each controller (nicely) what their
    // full state size is

    // mexPrintf("noutputs =  %i ", nlhs);

    // compute cond_state_dim
    int full_controller_sizes[n_comp];
    int full_controller_size = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        int n_cont = (xolotl_network.comp[i])->n_cont;

        full_controller_sizes[n_comp] = xolotl_network.comp[i]->getFullControllerSize();
        full_controller_size +=  full_controller_sizes[n_comp];
    }
    // mexPrintf("full controller state size =  %i ", full_controller_size);

    // compute ionic current state dimensions
    int full_current_size = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        full_current_size += (xolotl_network.comp[i])->n_cond;
    }

    // compute synaptic current state dimensions
    int full_synaptic_size = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        full_synaptic_size += (xolotl_network.comp[i])->n_syn;
    }

    // set up outputs as mex objects
    int res = dt/sim_dt;
    plhs[0] = mxCreateDoubleMatrix(param_size, 1, mxREAL);
    output_state = mxGetPr(plhs[0]);

    if (nlhs > 1) {
        plhs[1] = mxCreateDoubleMatrix(n_comp, nsteps_out, mxREAL);
        output_V = mxGetPr(plhs[1]);
    }
    if (nlhs > 2) {
        plhs[2] = mxCreateDoubleMatrix(2*n_comp, nsteps_out, mxREAL);
        output_Ca = mxGetPr(plhs[2]);
    }

    if (nlhs > 3) {
        plhs[3] = mxCreateDoubleMatrix(full_controller_size, nsteps_out, mxREAL);
        output_cont_state = mxGetPr(plhs[3]);
    }

    if (nlhs > 4) {
        plhs[4] = mxCreateDoubleMatrix(full_current_size, nsteps_out, mxREAL);
        output_curr_state = mxGetPr(plhs[4]);
    }

    if (nlhs > 5) {
        plhs[5] = mxCreateDoubleMatrix(full_synaptic_size, nsteps_out, mxREAL);
        output_syn_state = mxGetPr(plhs[5]);
    }

    // link up I_ext and V_clamp
    double * I_ext = new double[n_comp];
    double * V_clamp = new double[n_comp];
    double * I_ext_in = mxGetPr(prhs[1]);
    double * V_clamp_in = mxGetPr(prhs[2]);

    // copy I_ext so we can use it
    for(int q = 0; q < n_comp; q++)
    {
        I_ext[q] = I_ext_in[q];
        V_clamp[q] = V_clamp_in[q];
        // mexPrintf("I_ext =  %f ", I_ext_in[q]);
    }


    // figure out if we're voltage clamping
    bool is_voltage_clamped = false;
    for (int j = 0; j < n_comp; j++)
    {
        if (!isnan(V_clamp[j]))
        {
            is_voltage_clamped = true;
        }
    }

    // resolve the tree (for multi-compartment models)
    xolotl_network.resolveTree();

    if (is_voltage_clamped)
    {
        // do the integration respecting V_clamp
        int output_idx = 0;
        for(int i = 0; i < nsteps; i++)
        {


            for(int q = 0; q < n_comp; q++)
            {
                V_clamp[q] = V_clamp_in[i*n_comp + q];
            }

            xolotl_network.integrateClamp(sim_dt, V_clamp, delta_temperature);

            //xolotl:read_synapses_here
            //xolotl:read_controllers_here

            // here we're getting the state of every compartment -- V, Ca, and all conductances
            if (i%res == 0)
            {

                for (int j = 0; j < n_comp; j++)
                {

                    if (nlhs > 1) {
                        if (isnan(V_clamp[j]))
                        {
                            output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
                        }
                        else {
                            output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->I_clamp;
                        }
                    }

                    if (nlhs > 2) {
                        output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                        output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;
                    }

                    // cond_idx = 0;
                    // xolotl_network.comp[j]->get_cond_state(full_cond_state);

                    // get the states of every conductance
                    // for (int k = 0; k < cond_state_dims[j]; k++)
                    // {
                    //     output_cond_state[output_idx*cond_state_dim + cond_idx] = full_cond_state[k];
                    //     cond_idx ++;
                    // }
                }
                output_idx ++;
            }
        } // end for loop over nsteps
    }
    else

    {
        // voltage is not clamped
        // do the integration
        int output_idx = 0;
        int cont_idx = 0;
        int cond_idx = 0;
        int syn_idx = 0;
        for(int i = 0; i < nsteps; i++)
        {

            xolotl_network.integrate(sim_dt,I_ext, delta_temperature);

            //xolotl:read_synapses_here

            //xolotl:read_controllers_here

            // here we're getting the state of every compartment -- V, Ca, and all conductances
            if (i%res == 0)
            {

                for (int j = 0; j < n_comp; j++)
                {
                    // read out voltages
                    if (nlhs > 1) {
                        output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
                    }


                    // read out calcium + E_Ca
                    if (nlhs > 2) {
                        output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                        output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;
                    }

                    // read out controllers
                    if (nlhs > 3) {
                        cont_idx = (xolotl_network.comp[j]->getFullControllerState(output_cont_state,cont_idx));
                    }

                    // read out ionic currents
                    if (nlhs > 4)
                    {
                        cond_idx = (xolotl_network.comp[j]->getFullCurrentState(output_curr_state,cond_idx));
                    }

                    // read out synaptic currents
                    if (nlhs > 5)
                    {
                        syn_idx = (xolotl_network.comp[j]->getFullSynapseState(output_syn_state,syn_idx));
                    }

                }
                output_idx ++;
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
