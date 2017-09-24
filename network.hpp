//network class
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

    // constructor 
    network() {}
    
    // begin function declarations 
    void integrate(double);
    void integrateClamp(double, double);
    void addCompartment(compartment*);

};

// add a compartment to the network -- network is simply an array of pointers to compartments 
void network::addCompartment(compartment *comp_)
{
    comp.push_back(comp_);
}

// this integrate method works for networks
// of single compartments, or cells with
// multiple compartments under normal
// conditions. Don't use if something is 
// being voltage clamped! 
void network::integrate(double dt)
{
    int n_comp = (int) comp.size(); // these many compartments

    // integrate all channels in all compartments  
    for (int i = 0; i < n_comp; i++)
    { 
        double V_prev, Ca_prev;
        comp[i]->I_Ca = 0;
        comp[i]->I_ext = 0;

        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;
        comp[i]->integrateChannels(V_prev,Ca_prev,dt);

        // integrate synapses
        comp[i]->integrateSynapses(V_prev,dt);
    }

    // integrate all voltages and Ca in all compartments  
    for (int i = 0; i < n_comp; i++)
    { 
        double V_prev, Ca_prev;
        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;
        comp[i]->integrateVC(V_prev,Ca_prev,dt);
    }
}

// integrates a network of compartment, 
// and clamps the first compartment to V_clamp
void network::integrateClamp(double V_clamp, double dt)
{
    int n_comp = (int) comp.size(); // these many compartments

    // integrate all channels in all compartments  
    for (int i = 0; i < n_comp; i++)
    { 
        double V_prev, Ca_prev;
        comp[i]->I_Ca = 0;
        comp[i]->I_ext = 0;
        comp[i]->I_clamp = 0;

        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;
        comp[i]->integrateChannels(V_prev,Ca_prev,dt);

        // integrate synapses
        comp[i]->integrateSynapses(V_prev,dt);
    }

    // integrate all voltages and Ca in all compartments  
    for (int i = 0; i < n_comp; i++)
    { 
        double V_prev, Ca_prev;
        V_prev = comp[i]->V;
        Ca_prev = comp[i]->Ca;

        if (i == 0)
            comp[i]->integrateC_V_clamp(V_clamp,Ca_prev,dt);
        else
            comp[i]->integrateVC(V_prev,Ca_prev,dt);

        
    }
}


#endif



