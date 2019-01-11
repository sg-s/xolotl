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
    double *output_cont_state; // mechanisms
    double * spiketimes; 


    //xolotl:define_v_clamp_idx

    // make an empty network
    network xolotl_network;

    int n_synapses = 0;

    //xolotl:input_declarations


    // temperature and other wire-ups
    xolotl_network.temperature = temperature;
    xolotl_network.temperature_ref = temperature_ref;

    xolotl_network.sim_dt = sim_dt;
    xolotl_network.dt = dt;

    xolotl_network.verbosity = verbosity;
    xolotl_network.approx_channels = approx_channels;

    //xolotl:insert_constructors


    //xolotl:add_neurons_to_network

    //xolotl:add_conductances_here


    vector<synapse*> all_synapses; // pointers to all synapses 
    //xolotl:add_synapses_here


    //xolotl:add_mechanisms_here

    

    //xolotl:call_methods_here
    int nsteps = (int) floor(t_end/sim_dt);
    int progress_report = (int) floor(nsteps/10);

    int nsteps_out = (int) floor(t_end/dt);
    int n_comp = (int) (xolotl_network.comp).size(); // these many compartments


    // ask each controller (nicely) what their
    // full state size is
    int full_controller_sizes[n_comp];
    int full_controller_size = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        int n_cont = (xolotl_network.comp[i])->n_cont;

        full_controller_sizes[i] = xolotl_network.comp[i]->getFullMechanismSize();
        full_controller_size += full_controller_sizes[i];
    }
    

    // compute ionic current state dimensions
    // assumed to be the same for all conductances 
    int full_current_size = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        full_current_size += (xolotl_network.comp[i])->n_cond;
    }

    // compute synapse state dim
    int full_synaptic_sizes[n_comp];
    int full_synaptic_size = 0;
    for (int i = 0; i < n_comp; i ++)
    {
        int n_syn = (xolotl_network.comp[i])->n_syn;

        full_synaptic_sizes[i] = xolotl_network.comp[i]->getFullSynapseSize();
        full_synaptic_size += full_synaptic_sizes[i];
    }

    // set up outputs as mex objects
    int res = dt/sim_dt;
    if (verbosity > 0) {
        mexPrintf("[C++] res = %i\n",res);
        mexPrintf("[C++] nsteps = %i\n",nsteps);
    }
    


    // ask all the mechanisms for their sizes
    int begin_mechansism_sizes = param_size;
    param_size = param_size + n_mechanisms;
    

    plhs[0] = mxCreateDoubleMatrix(param_size, 1, mxREAL);
    output_state = mxGetPr(plhs[0]);

    int idx = 0;
    for(int j = 0; j < n_comp; j++) {
        for (int k = 0; k < xolotl_network.comp[j]->n_cont; k++) {
            int mech_size = (xolotl_network.comp[j]->getMechanismPointer(k))->getFullStateSize();
            output_state[begin_mechansism_sizes+idx] = mech_size;
            idx++;
        }
    }
         

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

    // return another output for spiketimes 
    if (nlhs > 6) {
        plhs[6] = mxCreateDoubleMatrix(nsteps_out, n_comp, mxREAL);
        spiketimes = mxGetPr(plhs[6]);
    }

    // link up I_ext and V_clamp
    double * I_ext = new double[n_comp];
    double * V_clamp = new double[n_comp];
    double * I_ext_in = mxGetPr(prhs[1]);
    double * V_clamp_in = mxGetPr(prhs[2]);


    // create arrays to help us find spikes
    // and return just the spiketimes
    double * prev_V = new double[n_comp]; 
    int * spike_time_idx = new int[n_comp]; 
    int spikes_only = 0;
    if (output_type == 2){
        // mexPrintf("Returning spiketimes instead of voltage..\n");
        spikes_only = 1;
        for (int i = 0; i < n_comp; i ++){
            spike_time_idx[i] = 0;
            prev_V[i] = -200;
        }
    }

    // figure out the sizes of the arrays 
    // for V_clamp and I_ext
    const mwSize *I_ext_dim, *V_clamp_dim;
    I_ext_dim = mxGetDimensions(prhs[1]);
    V_clamp_dim = mxGetDimensions(prhs[2]);

    int I_ext_size_1 = I_ext_dim[0];
    int I_ext_size_2 = I_ext_dim[1];
    int V_clamp_size_1 = V_clamp_dim[0];
    int V_clamp_size_2 = V_clamp_dim[1];



    if (verbosity > 0)
    {
        if (I_ext_size_2 == nsteps)
        {
            mexPrintf("[C++] dynamically changing I_ext\n");
        } else {
            mexPrintf("[C++] fixed I_ext\n");
        }

        if (V_clamp_size_2 == nsteps)
        {
            mexPrintf("[C++] dynamically changing V_clamp\n");
        } else {
            mexPrintf("[C++] fixed V_clamp\n");
        }
    }



    // copy I_ext so we can use it
    for(int q = 0; q < n_comp; q++)
    {
        I_ext[q] = I_ext_in[q];
        V_clamp[q] = V_clamp_in[q];
        // mexPrintf("I_ext =  %f ", I_ext_in[q]);
    }



    // determine mode
    bool is_voltage_clamped = false;
    bool is_multi_comp = xolotl_network.resolveTree();
    bool is_multi_step = false;
    if (solver_order > 0) {
        is_multi_step = true;
        xolotl_network.solver_order = (int) solver_order;

        // check that every component has a supported
        // solver that can work with this order
        xolotl_network.checkSolvers();
    }

    // figure out if we're voltage clamping
    // if any V_clamp is non-NaN, then we are
    
    for (int j = 0; j < n_comp; j++)
    {
        if (!isnan(V_clamp[j]))
        {
            is_voltage_clamped = true;
        }
    }


    mexEvalString("drawnow;");

    int percent_complete = 10;


    if (is_multi_comp & is_multi_step){
        mexErrMsgTxt("[xolotl] multi-compartment models cannot be integrated with multi-step methods yet. \n");
    }

    // if (is_multi_comp & is_voltage_clamped){
    //     mexErrMsgTxt("[xolotl] multi-compartment models cannot be integrated when something is clamped yet. \n");
    // }


    int output_idx = 0; 
    int cont_idx = 0;
    int cond_idx = 0;
    int syn_idx = 0;

    if (verbosity > 0){
        mexPrintf("[C++] %i outputs requested\n", nlhs);
    }
    

    // tell all components about some core 
    // parameters
    // xolotl_network.broadcast(sim_dt, temperature);

    if (!is_voltage_clamped & !is_multi_step){
 
        
        for (int i = 0; i < nsteps; i++) {
            if (I_ext_size_2 == nsteps) {
                // I_ext is dynamically changing
                for(int q = 0; q < n_comp; q++) {
                    I_ext[q] = I_ext_in[i*n_comp + q];
                }
            }

            xolotl_network.integrate(I_ext);


            if (i%progress_report == 0 & verbosity > 0) {
                mexPrintf("[C++] integration %i %", percent_complete);
                mexPrintf(" complete\n");
                percent_complete += 10;
                mexEvalString("drawnow;");
            }

            // here we're getting the state of every compartment -- V, Ca, and all conductances
            if (i%res == 0) {

                switch (nlhs)
                {
                    case 1:
                        // only one output, do nothing
                        break;
                    case 2:
                        // read out voltages only
                        for (int j = 0; j < n_comp; j++) {
                            output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
                        } // end j loop over compartments
                        break;

                    case 3:
                        // V + Ca
                        for (int j = 0; j < n_comp; j++)
                        {
                            output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
                            output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                            output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;

                        } // end j loop over compartments
                        break;

                    case 4:
                        // V, Ca, mechanisms

                        for (int j = 0; j < n_comp; j++)
                        {

                            output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
                            output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                            output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;
                            cont_idx = (xolotl_network.comp[j]->getFullMechanismState(output_cont_state,cont_idx));

                        } // end j loop over compartments
                        break;
                    

                    case 5:
                        // V, Ca, C, I
                        for (int j = 0; j < n_comp; j++)
                        {

                            output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
   
                            output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                            output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;
                            cont_idx = (xolotl_network.comp[j]->getFullMechanismState(output_cont_state,cont_idx));
                            cond_idx = (xolotl_network.comp[j]->getFullCurrentState(output_curr_state,cond_idx));

                        } // end j loop over compartments
                        break;



                    default:
                        // V, Ca, Ca, I, Syn
                        for (int j = 0; j < n_comp; j++)
                        {

                            output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
   
                            output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                            output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;
                            cont_idx = (xolotl_network.comp[j]->getFullMechanismState(output_cont_state,cont_idx));
                            cond_idx = (xolotl_network.comp[j]->getFullCurrentState(output_curr_state,cond_idx));

                        } // end j loop over compartments

                        for (int k = 0; k < n_synapses; k++)
                        {
                            syn_idx = (all_synapses[k]->getFullState(output_syn_state,syn_idx));
                        }

                        break;
                } // switch
                output_idx ++;



            } // if we need to write output



            // do we need to return spike times? 
            switch (spikes_only){

                case 0:
                    break;
                case 1:
                    // must return just spike times
                    for (int j = 0; j < n_comp; j++) {
                        if ((prev_V[j] < spike_thresh) && ((xolotl_network.comp[j]->V) > spike_thresh)) {
                            spiketimes[spike_time_idx[j] + j*nsteps_out] = i;
                            spike_time_idx[j]++;
                        } 
                        prev_V[j] = xolotl_network.comp[j]->V;
                    } // end j loop over compartments
                    break;
            }


                            

        } // end for loop over nsteps


    } else if (is_voltage_clamped & !is_multi_step) {

//                             _           _ 
//         _ __ ___   ___   __| | ___     / |
//        | '_ ` _ \ / _ \ / _` |/ _ \    | |
//        | | | | | | (_) | (_| |  __/    | |
//        |_| |_| |_|\___/ \__,_|\___|    |_|
                                


        // do the integration respecting V_clamp

        for (int i = 0; i < nsteps; i++) {

            if (V_clamp_size_2 == nsteps) {
                // I_ext is dynamically changing
                for(int q = 0; q < n_comp; q++) {
                    V_clamp[q] = V_clamp_in[i*n_comp + q];
                }
            }

            xolotl_network.integrateClamp(V_clamp);

            if (i%progress_report == 0 & verbosity > 0)
            {
                mexPrintf("[C++] integration %i %", percent_complete);
                mexPrintf(" complete\n");
                percent_complete += 10;
                mexEvalString("drawnow;");
            }

            // here we're getting the state of every compartment -- V, Ca, and all conductances
            if (i%res == 0)
            {
                for (int j = 0; j < n_comp; j++)
                {
                    if (nlhs > 1) {
                        if (isnan(V_clamp[j])) {
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
                }
                output_idx ++;
            }
        } // end for loop over nsteps


    } else if (!is_voltage_clamped & is_multi_step) {
//              
//                            _        ____  
//        _ __ ___   ___   __| | ___  |___ \
//       | '_ ` _ \ / _ \ / _` |/ _ \   __) |
//       | | | | | | (_) | (_| |  __/  / __/ 
//       |_| |_| |_|\___/ \__,_|\___| |_____|
                                    

        if (verbosity > 0) {
            mexPrintf("[xolotl] %i-step integration requested.\n", xolotl_network.solver_order);

        }
        



        for (int i = 0; i < nsteps; i++) {
            if (I_ext_size_2 == nsteps) {
                // I_ext is dynamically changing
                for(int q = 0; q < n_comp; q++) {
                    I_ext[q] = I_ext_in[i*n_comp + q];
                }
            }

            xolotl_network.integrateMS(I_ext);


            if (i%progress_report == 0 & verbosity > 0) {
                mexPrintf("[C++] integration %i %", percent_complete);
                mexPrintf(" complete\n");
                percent_complete += 10;
                mexEvalString("drawnow;");
            }



            // here we're getting the state of every compartment -- V, Ca, and all conductances
            if (i%res == 0) {

                for (int j = 0; j < n_comp; j++)
                {

                    if (nlhs > 1) {
                        output_V[output_idx*n_comp + j] = xolotl_network.comp[j]->V;
                    }

                    if (nlhs > 2) {
                        output_Ca[output_idx*2*n_comp + j] = xolotl_network.comp[j]->Ca;
                        output_Ca[output_idx*2*n_comp + j + n_comp] = xolotl_network.comp[j]->E_Ca;
                    }

                    if (nlhs > 3) {
                        cont_idx = (xolotl_network.comp[j]->getFullMechanismState(output_cont_state,cont_idx));
                    }

                    if (nlhs > 4) {
                        cond_idx = (xolotl_network.comp[j]->getFullCurrentState(output_curr_state,cond_idx));
                    }

                } // end j loop over compartments

                // read out synaptic currents and full
                // state of all synapses
                if (nlhs > 5) {
                    for (int k = 0; k < n_synapses; k++) {
                        syn_idx = (all_synapses[k]->getFullState(output_syn_state,syn_idx));
                    }
                }
                output_idx ++;
            } // end read out variables



            // do we need to return spike times? 
            switch (spikes_only){

                case 0:
                    break;
                case 1:
                    // must return just spike times
                    for (int j = 0; j < n_comp; j++) {
                        if ((prev_V[j] < spike_thresh) && ((xolotl_network.comp[j]->V) > spike_thresh)) {
                            spiketimes[spike_time_idx[j] + j*nsteps_out] = i;
                            spike_time_idx[j]++;
                        } 
                        prev_V[j] = xolotl_network.comp[j]->V;
                    } // end j loop over compartments
                    break;
            }


        } // end for loop over nsteps




    } else  {
        mexErrMsgTxt("[xolotl] Unknown integration mode. \n");
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
