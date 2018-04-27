// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Slave Integral Controller
// modification of the Integral controller
// as in O'Leary 2014 

#ifndef SUSHICONTROLLER
#define SUSHICONTROLLER
#include "../controller.hpp"

//inherit controller class spec
class SushiController: public controller {

protected:

    controller* downstream;
    controller* upstream;

public:
    // rates
    double downstream_rate;
    double upstream_rate; 
    double degradation_rate;
    double detachment_rate;

    // detached mRNA concentration 
    double m; 

    // mRNA in granules in compartment
    double m_granule; 

    // timescales
    double tau_m;
    double tau_g;


    // area of the container this is in
    // this is NOT necessarily the area of the compartment
    // that owns it. this will be set automatically 
    double container_A;

    // specify parameters + initial conditions for 
    // controller that controls a conductance 
    SushiController(double downstream_rate_, double upstream_rate_, double detachment_rate_, double degradation_rate_, double m_, double m_granule_, double tau_m_, double tau_g_)
    {

        downstream_rate = downstream_rate_;
        upstream_rate = upstream_rate_;
        detachment_rate = detachment_rate_;
        degradation_rate = degradation_rate_;
        m = m_;
        m_granule = m_granule_;

        tau_m = tau_m_;
        tau_g = tau_g_;


        // Null pointers for safety
        downstream = NULL;
        upstream = NULL;

        channel = NULL;
        syn = NULL;

        if (isnan (m)) { m = 0; }
        if (isnan (m_granule)) { m_granule = 0; }

        if (isnan (tau_m)) { tau_m = std::numeric_limits<double>::infinity(); }
        if (isnan (tau_g)) { tau_g = std::numeric_limits<double>::infinity(); }
        if (isnan (degradation_rate)) { degradation_rate = 0; }
        if (isnan (downstream_rate)) { downstream_rate = 0; }
        if (isnan (upstream_rate)) { upstream_rate = 0; }
        if (isnan (detachment_rate)) { detachment_rate = 0; }
        controlling_class = "unset";
    }

    
    void integrate(double Ca_error, double dt);
    void connect(conductance * channel_, synapse * syn_);
    void setMaster(controller * master_controller_);
    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int idx);
};


int SushiController::getFullStateSize()
{
    return 3; 
}


double SushiController::getState(int idx)
{
    if (idx == 1) {return m;}
    else if (idx == 2) {return m_granule;}
    else if (idx == 3) {return channel->gbar;}
    else {return std::numeric_limits<double>::quiet_NaN();}

}

int SushiController::getFullState(double *cont_state, int idx)
{
    // the full state of this controller has 3 dimensions:
    // 1 mRNA in granule form
    // 2 mRNA in compartment that can be translated
    // 3 protein level of controlled thing

    // 1 give it the granule mRNA level
    cont_state[idx] = m_granule;
    idx++;

    // 2 give it the current mRNA level
    cont_state[idx] = m;
    idx++;

    // 3 and also output the current gbar of the thing
    // being controller
    if (channel)
    {
      cont_state[idx] = channel->gbar;  
    }
    else if (syn)
    {
        cont_state[idx] = syn->gbar;  
    }
    idx++;
    return idx;
}


void SushiController::connect(conductance * channel_, synapse * syn_)
{
    if (channel_)
    {
        // connect to a channel
        channel = channel_;

        controlling_class = channel_->getClass();


        // attempt to read the area of the container that this
        // controller should be in. note that this is not necessarily the
        // container that contains this controller. rather, it is 
        // the compartment that contains the conductnace/synapse 
        // that this controller controls
        container_A  = (channel->container)->A;
        container_vol  = (channel->container)->vol;
    }
    if (syn_)
    {
        // connect to a synapse 
        syn = syn_;
    }
}

void SushiController::integrate(double Ca_error, double dt)
{

    // first, we wire up sushi controllers in 
    // different compartments controlling the 
    // various channels to each other 
    // using the magic of stream pointers 
    // (see network.resolveTree())
    if (!upstream && !downstream){
       
        // mexPrintf("master controller not defined\n");
        // attempt to ask the network for the right one
        // int n_comp = ((channel->container)->container)->n_comp;
        compartment * upstream_comp = (channel->container)->upstream;
        compartment * downstream_comp = (channel->container)->downstream;
        if (upstream_comp)
        {
            // there is an upstream compartment
            // ask that compartment (nicely) for a pointer
            // to the controller controlling the channel
            // that this one controls. we assume that it
            // is also a SushiController. if not, you will
            // get horrible errors. 

            upstream = upstream_comp->getControllerPointer(controlling_class.c_str());
        }

        if (downstream_comp)
        {
            // there is a downstream compartment. 
            // do the same thing
            downstream = downstream_comp->getControllerPointer(controlling_class.c_str());
        }
        return;
    }


    double mdot = 0;

    if ((channel->container)->tree_idx == 0)
    {
        // soma 
        mdot += Ca_error;
    }


    if (upstream)
    {
        mdot += downstream_rate*(upstream->getState(2)) - upstream_rate*m_granule;
    }


    if (downstream)
    {
        mdot += upstream_rate*(downstream->getState(2)) - downstream_rate*m_granule;
    }

    mdot -= detachment_rate*m_granule;

    // integrate mRNA on tracks 
    m_granule += (dt/tau_m)*mdot;



    // integrate detached mRNA in compartment 
    m += dt*(detachment_rate*m_granule - degradation_rate*m);

    // mRNA levels below zero don't make any sense
    if (m < 0) {m = 0;}
    if (m_granule < 0) {m_granule = 0;}



    // if (channel) {
    //     // channel is a non-NULL pointer, so
    //     // this controller must be controlling a 
    //     // channel
    //     // calculate conductance, not conductance density
        
    //     double g = (channel->gbar)*container_A;
    //     (channel->gbar) += ((dt/tau_g)*(m*container_vol - g))/container_A;

    //     // make sure it doesn't go below zero
    //     if ((channel->gbar) < 0) {
    //         (channel->gbar) = 0;
    //     }
    // }

    // if (syn) {
    //     // syn is a non-NULL pointer, so
    //     // this controller must be controlling a 
    //     // synpase that is presynaptic to this compartment 
    //     // calculate conductance, not conductance density
    //     double g = (syn->gbar); 

    //     // remember, that since everything is confusing,
    //     // synapses are in nS, and conductances are in 
    //     // uS/mm^2

    //     (syn->gbar) += ((dt/tau_g)*(m - g));

    //     // make sure it doesn't go below zero
    //     if ((syn->gbar) < 0) {
    //         (syn->gbar) = 0;
    //     }
    // }


}




#endif
