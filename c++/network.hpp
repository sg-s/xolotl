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

    // store pointers to all cable terminals 
    // each cable gets one terminal
    // from which we will start integration
    vector<compartment*> terminal_comp;

    // keeps track of the # of terminals in terminal_comp
    int n_terminals; 
public:
     // pointers to all compartments in network
    vector<compartment*> comp;



    // housekeeping
    int n_comp = 0;
    int n_soma = 0; // will be used in the Crank-Nicholson scheme
    double V_prev;
    double Ca_prev;

    // constructor
    network() {}

    // function declarations
    void integrate(double,double *, double);
    void integrateClamp(double, double *, double);
    void addCompartment(compartment*);
    void resolveTree(void);

};

void network::resolveTree(void)
{
    compartment * connected_comp = NULL;


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
                mexPrintf("found a soma, calling it = %i\n",n_soma);
                comp[i]->neuron_idx = n_soma;
                n_soma++;
                
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
                }
                else if ((connected_comp->tree_idx) == (ttl+1)) {
                    // connected_comp already has a tree_idx
                    // possibly manually entered, or from a previous
                    // integrate. if compatible, wire up stream
                    // pointers
                    (comp[i]->downstream) = connected_comp;
                    (connected_comp->upstream) = comp[i];

                    connected_comp->neuron_idx = comp[i]->neuron_idx;

                }
            }
        }
    }

    mexPrintf("n_soma = %i\n",n_soma);

    // now we make a list of terminals of all cables
    // we only keep track of one terminal -- which will
    // be arbitrarily chosen 
    int i = 0;
    while (i < n_soma)
    {
        // go over all compartments 
        for (int j = 0; j < n_comp; j ++)
        {
            if (isnan(comp[j]->neuron_idx)) {continue;}
            if ((comp[j]->neuron_idx) != i) {continue;}


            // it's possible to have a cable with 
            // terminals that have no downstream,
            // but at least one terminal of a cable 
            // will always have an upstream
            if (((comp[j]->n_axial_syn) == 1) && (comp[j]->upstream))
            {
                i++;
                terminal_comp.push_back(comp[j]);
            }

        }
    }


    // OK, now we have resolved the tree. 
    // now, we need to mark the downstream_g and 
    // upstream_g for every compartment
    connected_comp = NULL;
    int this_tree_level;
    for (int i = 0; i < n_comp; i ++)
    {

        if (comp[i]->n_axial_syn == 0) { continue;}
        if (isnan(comp[i]->tree_idx)) { continue;}


        this_tree_level = comp[i]->tree_idx;
        // go over every axial synapse
        for (int j = 0; j < comp[i]->n_axial_syn; j++)
        {
            if ((comp[i]->getConnectedCompartment(j))->tree_idx > this_tree_level)
            {
                // pre_syn is upstream of post_syn
                (comp[i]->axial_syn[j]->post_syn)->upstream_g = comp[i]->axial_syn[j]->gbar;
            } else {
                // pre_syn is downstream of post_syn
                (comp[i]->axial_syn[j]->post_syn)->downstream_g = comp[i]->axial_syn[j]->gbar;
            }

        }

        



    }

    n_terminals = terminal_comp.size();

}


// add a compartment to the network -- network contains
// a vector of pointers to compartments
void network::addCompartment(compartment *comp_)
{
    comp.push_back(comp_);
    n_comp++;
}

// this integrate method works for networks
// of single compartments, or cells with
// multiple compartments under normal
// conditions. Don't use if something is
// being voltage clamped!
void network::integrate(double dt, double * I_ext_now, double delta_temperature)
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

        comp[i]->i_Ca = 0;
        comp[i]->I_ext = I_ext_now[i];

        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;

        // integrate controllers
        comp[i]->integrateControllers(Ca_prev, dt);

        comp[i]->integrateChannels(V_prev, Ca_prev, dt, delta_temperature);

        // integrate synapses
        if (isnan(comp[i]->neuron_idx))
        {
            comp[i]->integrateSynapses(V_prev, dt, delta_temperature);
        } else {
            //comp[i]->integrateNonElectricalSynapses(V_prev, dt, delta_temperature);
        }
        
    }

    // integrate all voltages and Ca in all compartments
    for (int i = 0; i < n_comp; i++)
    {
        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;
        if (isnan(comp[i]->neuron_idx)) {
            comp[i]->integrateVC(V_prev, Ca_prev, dt, delta_temperature);
        } else {
            // this is a multi-compartment model,
            // so just integrate the calcium
            comp[i]->integrateCalcium(Ca_prev, dt);
        }
        
    }




    // OK, now we have integrated all single compartments,
    // but in multi compartment models, the 
    // voltages are un-integrated. since they are coupled to each
    // other, we use the Crank Nicholson scheme to integrate them
    // I will follow the scheme described in Dayan and Abbott (Chapter 6 Appendix B)
    // Convention will be the same, primes in the text will 
    // be replaced with _

    // for each cable 
    for (int nidx = 0; nidx < n_terminals; nidx++)
    {

        
        mexPrintf("going upstream\n");
        temp_comp = terminal_comp[nidx];


        // initialize 


        // go up the cable 
        while (temp_comp)
        {
            temp_comp->integrateCNForwardPass(dt);
            last_valid_comp = temp_comp;
            temp_comp = temp_comp->upstream;
            // nothing after this--temp_comp may be NULL
        }
        

        temp_comp = last_valid_comp;


        // go down the cable
        while (temp_comp)
        {

            temp_comp = temp_comp->downstream;
        }
    }
}

// integrate while clamping some compartments 
void network::integrateClamp(double dt, double *V_clamp, double delta_temperature)
{

    // integrate all channels in all compartments
    for (int i = 0; i < n_comp; i++)
    {
        comp[i]->i_Ca = 0;
        comp[i]->I_ext = 0;
        comp[i]->I_clamp = 0;

        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;
        comp[i]->integrateChannels(V_prev,Ca_prev,dt, delta_temperature);

        // integrate synapses
        comp[i]->integrateSynapses(V_prev,dt, delta_temperature);
    }

    // integrate all voltages and Ca in all compartments
    for (int i = 0; i < n_comp; i++)
    {
        
        Ca_prev = comp[i]->Ca;
        if (isnan(V_clamp[i])){
            V_prev = comp[i]->V;
            comp[i]->integrateVC(V_prev, Ca_prev, dt, delta_temperature);
        } else {
            comp[i]->integrateC_V_clamp(V_clamp[i], Ca_prev, dt, delta_temperature);
        }

    }
        
}


#endif
