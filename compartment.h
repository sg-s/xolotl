//single compartment class
#ifndef COMPARTMENT
#define COMPARTMENT
#include <cmath>
#include <vector>
#include "conductance.h"
#include "mex.h"

using namespace std;

/* single compartment class (unit capacitance)
 * contains vector of conductances
 * integrates membrane equation after
 * integrating each conductance state equation */
class compartment
{
protected:
    
    vector<conductance*> cond; // pointers to all conductances in compartment
    
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

    }
    
    // begin function declarations 
    // return v_mem
    double getV(void);
    double getCa(void);
    double integrate(double);
    void addConductance(conductance*);

};



double compartment::integrate(double dt)
{
    double V_prev = V;
    double Ca_prev = Ca;
    I_Ca = 0;

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

    // compute infinity values for V and Ca
    V_inf = sigma_gE/sigma_g;
    Ca_inf = Ca_in - f*A*I_Ca; // microM 

    // integrate V and Ca
    V = V_inf + (V_prev - V_inf)*exp(-dt/(Cm/(sigma_g)));
    Ca = Ca_inf + (Ca_prev - Ca_inf)*exp(-dt/tau_Ca);


    return 1;
}

// add conductance and provide pointer back to compartment 
void compartment::addConductance(conductance *cond_)
{
    cond.push_back(cond_);
    cond_->connect(this);
}



#endif



