/*

This document describes the "network" C++ class.
This class describes the network object, that is used
as an entry point for all other objects.

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| no |  compartment | nothing (root) |



*/


#ifndef NETWORK
#define NETWORK
#include <cmath>
#include <vector>

class compartment;


using namespace std;


class network
{
protected:
    // housekeeping
    compartment* temp_comp;
    compartment* last_valid_comp;

    // store pointers to all soma
    vector<compartment*> soma_comp;

    // keeps track of the # of terminals in terminal_comp
    int n_terminals;
public:


    double dt;
    double sim_dt;

     // pointers to all compartments in network
    vector<compartment*> comp;

    // temperature
    double temperature = 11;
    double temperature_ref = 11;

    // housekeeping
    int n_comp = 0;
    int n_soma = 0; // will be used in the Crank-Nicholson scheme

    int solver_order = 0;
    int stochastic_channels = 0;

    double verbosity;
    double approx_channels = 0;

    int use_current = 0;

    // constructor
    network() {}

    // function declarations
    void integrate(double *);
    void integrateMS(double *);
    void integrateClamp(double *);
    void integrateMSClamp(double *);
    void addCompartment(compartment*);
    bool resolveTree(void);
    void checkSolvers(void);
};


/*
This method verifies that all components can integrate
using the requested solver order. What this method does
is to call the `checkSolvers` in every compartment,
which in turn calls the `checkSolvers` method in every
component contained in every compartment.
*/
void network::checkSolvers(void) {
    // we assume that every component supports
    // the 0th order solver (Euler/exp Euler)
    if (solver_order == 0) { return;}

    for (int i = 0; i < n_comp; i ++){
        comp[i]->checkSolvers(solver_order);
    }

}

/*
This method "resolves" a multi-compartment neuron model.
What this means is that it works out which compartment
is "upstream" (closer to the soma) or "downstream" (further
from soma) for every compartment in a multi-compartment model.
It does so using the `tree_idx` property in every compartment,
setting it if need be.

It returns `true` if there is a multi-compartment neuron model
somewhere in the network, and `false` otherwise.

*/

bool network::resolveTree(void) {
    compartment * connected_comp = NULL;
    bool is_multi_comp = false;
    if (verbosity > 0) {
        mexPrintf("[C++] network resolve Tree called\n");
    }


    // ttl =  this_tree_level
    for (int ttl = 0; ttl < n_comp; ttl++) {
        // find all compartments with this_tree_level
        for (int i = 0; i < n_comp; i++) {
            if (isnan(comp[i]->tree_idx)) {continue;}
            if ((comp[i]->tree_idx) != ttl) {continue;}

            // OK, this compartment has the tree level we
            // are currently interested in

            if (comp[i]->tree_idx == 0) {
                // mexPrintf("found a soma, calling it = %i\n",n_soma);
                comp[i]->neuron_idx = n_soma;
                n_soma++;
                soma_comp.push_back(comp[i]);

            }

            // now go over every synapse that impinges on
            // this compartment and check if they're Axial
            // and if so get pointers to those presyn compartments

            for (int j = 0; j < comp[i]->n_axial_syn; j ++ ) {
                connected_comp = comp[i]->getConnectedCompartment(j);
                if (!connected_comp){continue;}
                if (isnan(connected_comp->tree_idx)) {
                    double child_tree_idx = ttl+1;

                    // set it
                    (connected_comp->tree_idx) = child_tree_idx;

                    // wire up stream pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;
                }
                else if ((connected_comp->tree_idx) == (ttl+1)) {
                    // connected_comp already has a tree_idx
                    // possibly manually entered, or from a previous
                    // integrate. if compatible, wire up stream
                    // pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;
                    is_multi_comp = true;

                }
            }
        }
    }



    // OK, now we have resolved the tree.
    // now, we need to mark the downstream_g and
    // upstream_g for every compartment
    for (int i = 0; i < n_comp; i ++) {
        comp[i]->resolveAxialConductances();
    }


    // go over every compartment, and check that stream
    // pointers and gs match up

    if (verbosity > 0) {
        for (int i = 0; i < n_comp; i++) {
            mexPrintf("---------------\n");
            mexPrintf("this comp tree_idx = %f\n",comp[i]->tree_idx);
            if (comp[i]->downstream) {
                mexPrintf("downstream pointer exists\n");

            } else {
                mexPrintf("NO downstream pointer\n");
            }
            mexPrintf("downstream_g =  %f\n", comp[i]->downstream_g);
            if (comp[i]->upstream) {
                mexPrintf("upstream pointer exists\n");

            } else {
                mexPrintf("No upstream pointer\n");
            }
            mexPrintf("upstream_g =  %f\n", comp[i]->upstream_g);

        }
    }

    return is_multi_comp;

}


/*
This method adds a compartment to the network. It does the following things:
1. adds a pointer to the compartment being added to a vector called `comp`
2. Broadcasts certain global parameters like `temperature`, `dt`, etc to all compartments.
3. Updates `n_comp` to that network knows how many compartments there are.
*/
void network::addCompartment(compartment *comp_) {
    comp.push_back(comp_);


    n_comp++;
    comp_->stochastic_channels = stochastic_channels;
    comp_->approx_channels = approx_channels;
    comp_->dt = sim_dt;
    comp_->verbosity = verbosity;
    comp_->RT_by_nF = (0.0431)*(temperature + 273.15);
    comp_->temperature = temperature;
    comp_->temperature_ref = temperature_ref;
    comp_->use_current = use_current;

}



/*
This method is used to integrate the network using a
multi-step Runge Kutta solver. This method assumes that
no compartment is being voltage clamped.
*/
void network::integrateMS(double * I_ext_now) {
    // first move all variables to prev state in all comps
    for (int i = 0; i < n_comp; i++) {
        comp[i]->V_prev = comp[i]->V;
        comp[i]->Ca_prev = comp[i]->Ca;
        comp[i]->i_Ca_prev = comp[i]->i_Ca;
        comp[i]->I_ext = I_ext_now[i];
    }

    for (int k = 0; k <= solver_order; k ++) {
        for (int i = 0; i < n_comp; i++) {
            comp[i]->integrateMS(k);
        }
    }
}

/*
This method is used to integrate the network using a
multi-step Runge-Kutta solver. This method assumes that
one compartment is being voltage-clamped.
*/

void network::integrateMSClamp(double * V_clamp) {
    // first move all variables to previous state in all compartments
    for (int i = 0; i < n_comp; i++) {
        comp[i]->V_prev = comp[i]->V;
        comp[i]->Ca_prev = comp[i]->Ca;
        comp[i]->i_Ca_prev = comp[i]->i_Ca;
    }

    // integrate using a multi-step solver
    for (int k = 0; k <= solver_order; k++) {
        for (int i = 0; i < n_comp; i++) {
            comp[i]->integrateMS(k);
        }
    }

    // set V_clamp in all the compartments
    for (int i = 0; i < n_comp; i++) {
        comp[i]->V_clamp = V_clamp[i];
    }
} // integrateMSClamp

/*
This method is used to integrate the network using the default
single step solver. Typically, this means using the exponential-
Euler method to integrate conductances and integrate the voltages
and Calcium levels in compartments, though mechanisms can implement
their own integration schemes. Multi-compartment models are
integrated using the Crank-Nicholson scheme.

This method assumes that no compartment anywhere is being voltage clamped.
*/
void network::integrate(double * I_ext_now) {

    // we will use Exponential Euler for single-compartment
    // models and networks, and Crank-Nicholson for
    // multi-compartment models. How do we know which is which?
    // all compartments with a neuron_idx will be assumed to
    // be part of a multi-compartment model
    // all compartments where neuron_idx is NaN will be treated
    // as single compartments and integrated normally


    // integrate all channels, mechanisms and synapses in all compartments
    for (int i = 0; i < n_comp; i++) {

        // move current values to previous values
        comp[i]->V_prev = comp[i]->V;
        comp[i]->Ca_prev = comp[i]->Ca;
        comp[i]->i_Ca_prev = comp[i]->i_Ca;

        comp[i]->i_Ca = 0;
        comp[i]->I_ext = I_ext_now[i];


        comp[i]->integrateMechanisms();

        comp[i]->integrateChannels();

        comp[i]->integrateSynapses();


    }

    // integrate voltages in all single compartments
    // and calcium in all compartments
    for (int i = 0; i < n_comp; i++) {
        if (isnan(comp[i]->neuron_idx)) {
            comp[i]->integrateVoltage();
        } else {
            // this is a multi-compartment model,
            // so just integrate the calcium
            // this is assumed to be already done in the
            // mechanism integration, so do nothing here
        }

    }

    // OK, now we have integrated all single compartments,
    // but in multi compartment models, the
    // voltages are un-integrated. since they are coupled to each
    // other, we use the Crank Nicholson scheme to integrate them
    // I will follow the scheme described in Dayan and Abbott
    // (Chapter 6 Appendix B)
    // Convention will be the same, primes in the text will
    // be replaced with _
    // integration starts at the soma (mu = 0), and proceeds
    // down the cable

    // for each cable
    for (int nidx = 0; nidx < n_soma; nidx++) {


        temp_comp = soma_comp[nidx];

        // go down the cable -- away from soma
        // to terminal, increasing in tree_idx
        while (temp_comp) {
            temp_comp->integrateCNFirstPass();
            last_valid_comp = temp_comp;
            temp_comp = temp_comp->downstream;
            // nothing after this--temp_comp may be NULL
        }


        temp_comp = last_valid_comp;


        // go up the cable -- towards soma
        // away from terminal, decreasing in tree_idx
        while (temp_comp) {
            temp_comp->integrateCNSecondPass();
            temp_comp = temp_comp->upstream;
            // nothing after this, becaue temp_comp may be NULL
        }
    }
}

/*
This method is used to integrate the network using the default
single step solver. Typically, this means using the exponential-
Euler method to integrate conductances and integrate the voltages
and Calcium levels in compartments, though mechanisms can implement
their own integration schemes. Multi-compartment models are
integrated using the Crank-Nicholson scheme.

This method assumes that some compartment is being voltage clamped,
and also assumes that no current is being injected into any compartment.
*/
void network::integrateClamp(double *V_clamp) {

    // integrate all channels in all compartments
    for (int i = 0; i < n_comp; i++) {

        // move current values to previous values
        comp[i]->V_prev = comp[i]->V;
        comp[i]->Ca_prev = comp[i]->Ca;
        comp[i]->i_Ca_prev = comp[i]->i_Ca;

        // reset values
        comp[i]->i_Ca = 0;
        comp[i]->I_ext = 0;
        comp[i]->I_clamp = 0;

        comp[i]->integrateMechanisms();
        comp[i]->integrateChannels();

        // integrate synapses
        if (isnan(comp[i]->neuron_idx)) {
            comp[i]->integrateSynapses();
        }
    }


    // set V_clamp in all the compartments
    for (int i = 0; i < n_comp; i++) {
        comp[i]->V_clamp = V_clamp[i];
    }



    // integrate all voltages in all single compartments
    for (int i = 0; i < n_comp; i++) {
        if (isnan(comp[i]->neuron_idx)) {
            if (isnan(V_clamp[i])){
                // not being clamped
                comp[i]->integrateVoltage();
            } else {
                comp[i]->integrateV_clamp(V_clamp[i]);
            }
        }
    } // end for loop over compartments


    // now integrate the multi-compartments
    // for each cable
    for (int nidx = 0; nidx < n_soma; nidx++) {

        temp_comp = soma_comp[nidx];

        // go down the cable -- away from soma
        // to terminal, increasing in tree_idx
        while (temp_comp) {
            temp_comp->integrateCNFirstPass();
            last_valid_comp = temp_comp;
            temp_comp = temp_comp->downstream;
            // nothing after this--temp_comp may be NULL
        }


        temp_comp = last_valid_comp;

        // go up the cable -- towards soma
        // away from terminal, decreasing in tree_idx
        while (temp_comp) {
            if (isnan(temp_comp->V_clamp)) {

                temp_comp->integrateCNSecondPass();

            } else {
                // try to compute the clamping current
                // this is probably only approximately correct
                temp_comp->computeClampingCurrent(temp_comp->V_clamp);
            }


            temp_comp = temp_comp->upstream;
            // nothing after this, because temp_comp may be NULL
        }
    }

} // integrateClamp


#endif
