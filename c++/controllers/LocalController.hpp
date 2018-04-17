// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Local Integral Controller
// modification of the Integral controller
// as in O'Leary 2014 
// in this controller, the rates of insertion
// and degradation of proteins are 
// Hill functions of the local Calcium conc. 
// this controller supports both Hill functions
// that increase and decrease with local calcium
// using a sign trick: 
//
// to use Hill functions that increase with
// local calcium, specify a +ve K_d
// to use Hill functions that decrease with 
// local calcium, specify a -ve K_d

#ifndef LOCALCONTROLLER
#define LOCALCONTROLLER
#include "../controller.hpp"

//inherit controller class spec
class LocalController: public controller {

protected:
public:
    // timescales
    double tau_m;
    double tau_g; 

    // mRNA concentration 
    double m; 

    // synthesis parameters 
    double K_syn;
    double n_syn;

    // degradation parameters
    double K_deg;
    double n_deg;

    // fudge factor
    double phi;


    // area of the container this is in
    // this is NOT necessarily the area of the compartment
    // that contains it
    double container_A;


    // housekeeping
    double Alpha;
    double Gamma;

    // specify parameters + initial conditions for 
    // controller that controls a conductance 
    LocalController(double tau_m_, double tau_g_, double m_, double phi_, double K_syn_, double K_deg_, double n_syn_, double n_deg_)
    {

        tau_m = tau_m_;
        tau_g = tau_g_;
        m = m_;
        phi = phi_;

        K_syn = K_syn_;
        K_deg = K_deg_;
        n_syn = n_syn_;
        n_deg = n_deg_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (phi)) { phi = 1; }
        if (isnan (n_syn)) { n_syn = 1; }
        if (isnan (n_deg)) { n_deg = 1; }

    }

    
    void integrate(double Ca_error, double dt);
    void connect(conductance * channel_, synapse * syn_);
    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);

};

int LocalController::getFullStateSize()
{
    return 2; 
}


int LocalController::getFullState(double *cont_state, int idx)
{
    // give it the current mRNA level
    cont_state[idx] = m;

    idx++;

    // and also output the current gbar of the thing
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


void LocalController::connect(conductance * channel_, synapse * syn_)
{
    if (channel_)
    {
        // connect to a channel
        channel = channel_;

        // attempt to read the area of the container that this
        // controller should be in. note that this is not necessarily the
        // container that contains this controller. rather, it is 
        // the compartment that contains the conductance/synapse 
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

void LocalController::integrate(double Ca_error, double dt)
{
    // integrate mRNA
    m += (dt/tau_m)*(Ca_error);

    // mexPrintf("m =  %f\n",m);

    // mRNA levels below zero don't make any sense
    if (m < 0) {
        m = 0;
    }


    if (channel) {
        // channel is a non-NULL pointer, so
        // this controller must be controlling a 
        // channel
        // calculate conductance, not conductance density
        
        // measure the local calcium concentration 
        double Ca = (channel->container)->Ca;

        if (K_syn < 0) {Alpha = 1/(1+pow(Ca/(-K_syn),n_syn));}
        else { Alpha = 1/(1+pow(K_syn/Ca,n_syn)); }

        if (K_deg < 0) {Gamma = 1/(1+pow(Ca/(-K_deg),n_deg));}
        else { Gamma = 1/(1+pow(K_deg/Ca,n_deg)); }


        double g = (channel->gbar)*container_A;
        (channel->gbar) += ((dt/tau_g)*(m*container_vol*Alpha*phi - Gamma*g))/container_A;

        // make sure it doesn't go below zero
        if ((channel->gbar) < 0) {
            (channel->gbar) = 0;
        }
    }

    if (syn) {
        // syn is a non-NULL pointer, so
        // this controller must be controlling a 
        // synpase that is presynaptic to this compartment 
        // calculate conductance, not conductance density
        double g = (syn->gbar); 

        // remember, that since everything is confusing,
        // synapses are in nS, and conductances are in 
        // uS/mm^2

        (syn->gbar) += ((dt/tau_g)*(m - g));

        // make sure it doesn't go below zero
        if ((syn->gbar) < 0) {
            (syn->gbar) = 0;
        }
    }


}

// return the mRNA level, because this is a protected
// member 
double LocalController::get_m(void)
{
    return m;
}

// return the conductance of either the 
// channel or the synapse that this 
// controller is controlling 
double LocalController::get_gbar(void)
{

    double gbar;
    if (channel) {
        gbar = channel->gbar;
    }
    if (syn) {
        gbar = syn->gbar;
    }
    return gbar;
}


#endif
