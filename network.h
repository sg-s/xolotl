//network class
#ifndef NETWORK
#define NETWORK
#include <cmath>
#include <vector>
#include "compartment.h"
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
    void addCompartment(compartment*);

};

void network::integrate(double dt)
{
    int n_comp = (int) comp.size(); // these many compartments

    //mexPrintf("n_comp: %d\n",n_comp);

    // integrate all channels in all compartments  
    for (int i = 0; i < n_comp; i++)
    { 
        double V_prev, Ca_prev;
        comp[i]->I_Ca = 0;
        comp[i]->I_ext = 0;
        //mexPrintf("I_ext of this cell = %f\n",comp[i]->I_ext);
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

// add a compartment to the network -- network is simply an array of pointers to compartments 
void network::addCompartment(compartment *comp_)
{
    comp.push_back(comp_);
    //cond_->connect(this);
}



#endif



