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

    nsteps = (int) floor(t_end/sim_dt);
    int n_comp = (int) (xolotl_network.comp).size(); // these many compartments


    plhs[0] = mxCreateDoubleMatrix(1, param_size, mxREAL); //output state

    output_state = mxGetPr(plhs[0]);


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
        // do the integration
        for(int i = 0; i < nsteps; i++)
        {
            xolotl_network.integrateClamp(sim_dt,V_clamp, delta_temperature);
        }
    }
    else // not being voltage clamped 
    {
        // do the integration
        for(int i = 0; i < nsteps; i++)
        {
            xolotl_network.integrate(sim_dt,I_ext, delta_temperature);
        }
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
