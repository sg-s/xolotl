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

public:
    vector<compartment*> comp; // pointers to all compartments in network

    // housekeeping
    int n_comp = 0;
    double V_prev;
    double Ca_prev;

    // constructor
    network() {}

    // begin function declarations
    void integrate(double,double *, double);
    void integrateClamp(double, double *, double);
    void addCompartment(compartment*);
    compartment* findSoma(void);

};

compartment* network::findSoma(void)
{
    compartment * c = NULL;

    // first, find compartments that are soma 
    for (int i = 0; i < n_comp; i++)
    {
        if ((comp[i]->tree_idx) == 0)
        {
            c = comp[i];
        }
    }
    return c;

}

// add a compartment to the network -- network is simply an array of pointers to compartments
void network::addCompartment(compartment *comp_)
{
    comp.push_back(comp_);
    n_comp++;
    comp_->connect(this);
}

// this integrate method works for networks
// of single compartments, or cells with
// multiple compartments under normal
// conditions. Don't use if something is
// being voltage clamped!
void network::integrate(double dt,double * I_ext_now, double delta_temperature)
{
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
        comp[i]->integrateSynapses(V_prev, dt, delta_temperature);
    }

    // integrate all voltages and Ca in all compartments
    for (int i = 0; i < n_comp; i++)
    {
        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;
        comp[i]->integrateVC(V_prev, Ca_prev, dt, delta_temperature);
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
        if (isnan(V_clamp[i]))
        {
            V_prev = comp[i]->V;
            comp[i]->integrateVC(V_prev, Ca_prev, dt, delta_temperature);
        }
        else 
        {
            comp[i]->integrateC_V_clamp(V_clamp[i], Ca_prev, dt, delta_temperature);
        }

    }
        
}


#endif
