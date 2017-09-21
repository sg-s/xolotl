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

    // make an empty network 
    network STG;

    int nits;

    // wire up simulation parameters
    double * simparams = mxGetPr(prhs[0]);

    double dt  = simparams[0];
    double tstop = simparams[1];

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
    output_V = mxGetPr(plhs[0]);
    output_Ca = mxGetPr(plhs[1]);
    
    for(int i = 0; i < nits; i++)
    {
        STG.integrate(dt);
        for (int j = 0; j < n_comp; j++)
        {
            output_V[i*n_comp + j] = STG.comp[j]->V;
            output_Ca[i*n_comp + j] = STG.comp[j]->Ca;
        }

    }
}
