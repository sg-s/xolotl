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


    //xolotl:define_v_clamp_idx

    // make an empty network
    network xolotl_network;

    int nsteps;
    int n_synapses = 0; // keeps track of how many synapses we have

    //xolotl:input_declarations


    double delta_temperature = (temperature - temperature_ref)/10;

    //xolotl:insert_constructors

    //xolotl:add_conductances_here

    //xolotl:add_synapses_here

    //xolotl:add_controllers_here

    //xolotl:add_neurons_to_network

    nsteps = (int) floor(t_end/dt);
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

    plhs[0] = mxCreateDoubleMatrix(1, param_size, mxREAL); //output state

    output_state = mxGetPr(plhs[0]);


    // make arrays which will store the full cond. state

    int cond_idx = 0;
    double * I_ext_now = new double[n_comp];
    // make sure I_ext_now is zero
    for(int q = 0; q < n_comp; q++)
    {
        I_ext_now[q] = 0.0;
    }

    // do the integration
    for(int i = 0; i < nsteps; i++)
    {
        for(int j = 0; j < n_comp; j++)
        {
            //xolotl:enable_when_I_ext
            //I_ext_now[j] = I_ext[i];
        }
        //xolotl:disable_when_clamped
        xolotl_network.integrate(dt,I_ext_now, delta_temperature);
        //xolotl:enable_when_clamped
        //xolotl_network.integrateClamp(V_clamp[i],dt, delta_temperature);
        //xolotl:enable_when_clamped
        //output_I_clamp[i] = xolotl_network.comp[0]->I_clamp;

        //xolotl:read_synapses_here

        //xolotl:read_controllers_here

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
