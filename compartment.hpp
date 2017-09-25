//single compartment class
#ifndef COMPARTMENT
#define COMPARTMENT
#include <cmath>
#include <vector>
#include "conductance.hpp"
#include "synapse.hpp"

using namespace std;

/* single compartment class (unit capacitance)
 * contains vector of conductances
 * integrates membrane equation after
 * integrating each conductance state equation */
class compartment
{
protected:
    
    vector<conductance*> cond; // pointers to all conductances in compartment
    vector<synapse*> syn; // pointers to synapses onto this neuron. 
    
    // voltage and other state variables (calcium, ..
    double sigma_g;
    double sigma_gE;
    double V_inf;         
    double Ca_inf; 

    // neuron parameters
    double Cm; // specific capacitance 
    double A; 
    double f; 
    double Ca_in;
    double Ca_out; 
    double tau_Ca;
    double RT_by_nF;

public:
    double V;          
    double Ca; 
    double E_Ca;
    double I_Ca;
    double I_ext; // all external currents are summed here
    double I_clamp; // this is the current required to clamp it 
    int n_cond; // this keep tracks of the # channels

    // constructor with all parameters 
    compartment(double V_, double Ca_, double Cm_, double A_, double f_, double Ca_out_, double Ca_in_, double tau_Ca_)
    {
        V = V_;
        Ca = Ca_;

        sigma_g = 0;
        sigma_gE = 0;
        V_inf = 0;
        Ca_inf = 0;

        Cm = Cm_;
        A = A_; 
        f = f_; 
        Ca_in = Ca_in_; 
        Ca_out = Ca_out_;
        tau_Ca = tau_Ca_;

        RT_by_nF = 500.0*(8.6174e-5)*(10 + 273.15);

        // housekeeping
        E_Ca = 0;
        I_Ca = 0; // this is the current density (nA/mm^2)
        n_cond = 0;

    }
    // begin function declarations 
    void addConductance(conductance*);
    void addSynapse(synapse*);
    void integrate(double);
    void integrateChannels(double, double, double);
    void integrateSynapses(double, double);
    void integrateVC(double, double, double);
    void integrateC_V_clamp(double, double, double);
    void get_cond_state(double*);

};

// simple integrate; use this only for 
// single-compartment non-clamped neurons
void compartment::integrate(double dt)
{
    double V_prev = V;
    double Ca_prev = Ca;
    I_Ca = 0;
    I_ext = 0;
    I_clamp = 0;

    integrateChannels(V_prev, Ca_prev, dt);
    integrateVC(V_prev, Ca_prev, dt);

}

void compartment::integrateChannels(double V_prev, double Ca_prev, double dt)
{
    int n_cond = (int) cond.size(); //conductances
    sigma_g = 0.0;
    sigma_gE = 0.0;


    // compute E_Ca
    E_Ca = RT_by_nF*log((Ca_out)/(Ca_prev));

    // integrate all channels
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->integrate(V_prev, Ca_prev, dt);
        sigma_g += cond[i]->g;
        sigma_gE += (cond[i]->g)*(cond[i]->E);
    }
}

void compartment::integrateSynapses(double V_prev, double dt)
{

    int n_syn = (int) syn.size(); //these many synapses
    
    // integrate all synapses
    for (int i=0; i<n_syn; i++)
    {
        // mexPrintf("integrating synapse in comp: =  %i\n",&(syn[i]));
        syn[i]->integrate(dt);
        I_ext += (syn[i]->getCurrent(V_prev));

    }
}

void compartment::integrateVC(double V_prev, double Ca_prev, double dt)
{
    // compute infinity values for V and Ca
    //mexPrintf("sigma_gE =  %f\n",sigma_gE);
    //mexPrintf("I_ext/A =  %f\n",I_ext/A);
    V_inf = (sigma_gE + (I_ext/A))/sigma_g;
    Ca_inf = Ca_in - f*A*I_Ca; // microM 

    // integrate V and Ca
    V = V_inf + (V_prev - V_inf)*exp(-dt/(Cm/(sigma_g)));
    Ca = Ca_inf + (Ca_prev - Ca_inf)*exp(-dt/tau_Ca);
}


// integrates calcium, but not voltage
// assumes cell is voltage clamped, and 
// returns the current required to clamp it
void compartment::integrateC_V_clamp(double V_clamp, double Ca_prev, double dt)
{
    // compute infinity values for Ca
    Ca_inf = Ca_in - f*A*I_Ca; // microM 

    // integrate V and Ca
    I_clamp = -I_ext + A*(V_clamp*sigma_g - sigma_gE);
    V = V_clamp;
    Ca = Ca_inf + (Ca_prev - Ca_inf)*exp(-dt/tau_Ca);
}


// add conductance and provide pointer back to compartment 
void compartment::addConductance(conductance *cond_)
{
    cond.push_back(cond_);
    cond_->connect(this);
    n_cond += 1;
}

// add synapse to this compartment (this compartment is after synapse)
void compartment::addSynapse(synapse *syn_)
{
    syn.push_back(syn_);
}

void compartment::get_cond_state(double *cond_state)
{
    for (int i = 0; i < n_cond; i ++) 
    {
        cond_state[i*2] = cond[i]->m;
        cond_state[(i*2)+1] = cond[i]->h;
    }
}



#endif



