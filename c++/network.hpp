// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// class that defines a network
// a network can either be a network of
// single compartment neurons, or a
// multi-compartment neuron

#ifndef NETWORK
#define NETWORK
#include <cmath>
#include <vector>
#include "compartment.hpp"
#include "mex.h"

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

     // pointers to all compartments in network
    vector<compartment*> comp;

    // temperature
    double temperature;
    double temperature_ref;

    // housekeeping
    int n_comp = 0;
    int n_soma = 0; // will be used in the Crank-Nicholson scheme

    int solver_order = 0;

    double verbosity;
    double approx_channels = 0;

    // constructor
    network() {}

    // function declarations
    void integrate(double *);
    void integrateMS(double *);
    void integrateClamp(double *);
    void addCompartment(compartment*);
    bool resolveTree(void);

    void checkSolvers(void);

    void broadcast(double, double);

};

// broadcast is a method that tells all 
// components of a network about some 
// important parameters that are not
// going to change
void network::broadcast(double dt, double temperature)
{
    dt = dt;
    for (int i = 0; i < n_comp; i ++) 
    {
        comp[i]->dt = dt;
        for (int j = 0; j < comp[i]->n_cond; j ++) {
            (comp[i]->getConductancePointer(j))->dt = dt; 
            (comp[i]->getConductancePointer(j))->temperature = temperature;

            // built look up table if asked to 
            // approximate gating functions 

            (comp[i]->getConductancePointer(j))->buildLUT(approx_channels);
  
            
        }
        for (int j = 0; j < comp[i]->n_cont; j ++) {
            (comp[i]->getMechanismPointer(j))->dt = dt;
            (comp[i]->getMechanismPointer(j))->temperature = temperature;
        }
        for (int j = 0; j < comp[i]->n_syn; j ++) 
        {
            (comp[i]->getSynapsePointer(j))->dt = dt;
            (comp[i]->getSynapsePointer(j))->temperature = temperature;
        }

    }
}

// this method checks that every component
// in the network has a solver that supports
// the requested order. if it doesn't, it 
// throws an error 
void network::checkSolvers(void)
{
    if (solver_order == 0) { return;}

    for (int i = 0; i < n_comp; i ++){
        comp[i]->checkSolvers(solver_order);
    }

}

bool network::resolveTree(void) {
    compartment * connected_comp = NULL;
    bool is_multi_comp = false;
    if (verbosity > 0)
    {
        mexPrintf("[C++] network::resolveTree() called\n");
    }


    // ttl =  this_tree_level
    for (int ttl = 0; ttl < n_comp; ttl++)
    {
        // find all compartments with this_tree_level
        for (int i = 0; i < n_comp; i++)
        {
            if (isnan(comp[i]->tree_idx)) {continue;}
            if ((comp[i]->tree_idx) != ttl) {continue;}

            // OK, this compartment has the tree level we 
            // are currently interested in 

            if (comp[i]->tree_idx == 0)
            {
                // mexPrintf("found a soma, calling it = %i\n",n_soma);
                comp[i]->neuron_idx = n_soma;
                n_soma++;
                soma_comp.push_back(comp[i]);
                
            }

            // now go over every synapse that impinges on 
            // this compartment and check if they're Axial
            // and if so get pointers to those presyn compartments

            for (int j = 0; j < comp[i]->n_axial_syn; j ++ )
            {
                connected_comp = comp[i]->getConnectedCompartment(j);
                if (!connected_comp){continue;}
                if (isnan(connected_comp->tree_idx))
                {
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
    
    if (verbosity > 0)
    {
        for (int i = 0; i < n_comp; i++)
        {
            mexPrintf("---------------\n");
            mexPrintf("this comp tree_idx = %f\n",comp[i]->tree_idx);
            if (comp[i]->downstream)
            {
                mexPrintf("downstream pointer exists\n");
                
            } else {
                mexPrintf("NO downstream pointer\n");
            }   
            mexPrintf("downstream_g =  %f\n", comp[i]->downstream_g);
            if (comp[i]->upstream)
            {
                mexPrintf("upstream pointer exists\n");

            } else {
                mexPrintf("No upstream pointer\n");
            }
            mexPrintf("upstream_g =  %f\n", comp[i]->upstream_g);

        }
    }

    return is_multi_comp;

}


// add a compartment to the network -- network contains
// a vector of pointers to compartments
void network::addCompartment(compartment *comp_) {
    comp.push_back(comp_);
    n_comp++;
    comp_->verbosity = verbosity;
    comp_->RT_by_nF = (0.0431)*(temperature + 273.15);

    if (verbosity > 0)
    {
        mexPrintf("[C++] adding compartment to network. \n");
    }
}



// this integrate method is meant to use
// a multi-step solver like a rk4 solver
// this requires there to exist a solver
// for every component that supports this 
// order
void network::integrateMS(double * I_ext_now) {
    // first move all variables to prev state in all comps
    for (int i = 0; i < n_comp; i++)
    {
        comp[i]->V_prev = comp[i]->V;
        comp[i]->Ca_prev = comp[i]->Ca;
        comp[i]->i_Ca_prev = comp[i]->i_Ca;
        comp[i]->I_ext = I_ext_now[i];
    }

    for (int k = 0; k <= solver_order; k ++)
    {
        for (int i = 0; i < n_comp; i++)
        {
            comp[i]->integrateMS(k);
        }
    }
}


// this integrate method works for networks
// of single compartments, or cells with
// multiple compartments under normal
// conditions. Don't use if something is
// being voltage clamped!
void network::integrate(double * I_ext_now)
{

    // we will use Exponential Euler for single-compartment
    // models and networks, and Crank-Nicholson for 
    // multi-compartment models. How do we know which is which?
    // all compartments with a neuron_idx will be assumed to
    // be part of a multi-compartment model
    // all compartments where neuron_idx is NaN will be treated
    // as single compartments and integrated normally 

    // integrate all channels in all compartments
    for (int i = 0; i < n_comp; i++)
    {

        // move current values to previous values
        comp[i]->V_prev = comp[i]->V;
        comp[i]->Ca_prev = comp[i]->Ca;
        comp[i]->i_Ca_prev = comp[i]->i_Ca;

        comp[i]->i_Ca = 0;
        comp[i]->I_ext = I_ext_now[i];
        
        // integrate controllers
        comp[i]->integrateMechanisms();

        comp[i]->integrateChannels();

        

        // integrate synapses
        if (isnan(comp[i]->neuron_idx))
        {
            comp[i]->integrateSynapses();
        }
        
    }

    // integrate voltages in all single compartments
    // and calcium in all compartments
    for (int i = 0; i < n_comp; i++)
    {
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
    for (int nidx = 0; nidx < n_soma; nidx++)
    {

        
        temp_comp = soma_comp[nidx];


        // go down the cable -- away from soma
        // to terminal, increasing in tree_idx
        while (temp_comp)
        {
            temp_comp->integrateCNFirstPass();
            last_valid_comp = temp_comp;
            temp_comp = temp_comp->downstream;
            // nothing after this--temp_comp may be NULL
        }
        

        temp_comp = last_valid_comp;


        // go up the cable -- towards soma
        // away from terminal, decreasing in tree_idx
        while (temp_comp)
        {
            temp_comp->integrateCNSecondPass();
            temp_comp = temp_comp->upstream;
            // nothing after this, becaue temp_comp may be NULL
        }
    }
}

// integrate while voltage clamping some compartments 
void network::integrateClamp(double *V_clamp)
{

    // integrate all channels in all compartments
    for (int i = 0; i < n_comp; i++)
    {

        // move current values to previous values
        comp[i]->V_prev = comp[i]->V;
        comp[i]->Ca_prev = comp[i]->Ca;
        comp[i]->i_Ca_prev = comp[i]->i_Ca;


        comp[i]->i_Ca = 0;
        comp[i]->I_ext = 0;
        comp[i]->I_clamp = 0;

        comp[i]->integrateChannels();

        // integrate synapses
        comp[i]->integrateSynapses();
    }

    // integrate all voltages and Ca in all compartments
    for (int i = 0; i < n_comp; i++)
    {
        
        if (isnan(V_clamp[i])){

            comp[i]->integrateVoltage();
        } else {
            comp[i]->integrateV_clamp(V_clamp[i]);
        }

    }
        
}


#endif
