// _  _ ____ _    ____ ___ _    
//  \/  |  | |    |  |  |  |    
// _/\_ |__| |___ |__|  |  |___ 
//
// Calcium mechanism
//  
// as in Bucholtz et al. 1992
// and in "Methods in neuronal modelling"
// and others. 

#ifndef CALCIUMMECH2
#define CALCIUMMECH2
#include "mechanism.hpp"
#include <limits>


//inherit controller class spec
class CalciumMech2: public mechanism {

protected:
public:


    // parameters for calciumMech2 
    double tau_Ca = 200;  // milliseconds

    // parameter to convert from i_Ca to uM 
    double phi = .1; 

    double Ca_in = .05;

    // specify parameters + initial conditions for 
    // mechanism that controls a conductance 
    CalciumMech2(double tau_Ca_, double phi_, double Ca_in_)
    {
        phi = phi_;
        tau_Ca = tau_Ca_;
        Ca_in = Ca_in_;

        controlling_class = "unset";

    }

    
    void integrate(void);
    void integrateMS(int, double, double);

    void checkSolvers(int);

    void connect(compartment * comp_);
    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);

    double Cadot(double);

};


double CalciumMech2::getState(int idx)
{

    return std::numeric_limits<double>::quiet_NaN();

}


int CalciumMech2::getFullStateSize()
{
    return 0; 
}


int CalciumMech2::getFullState(double *cont_state, int idx)
{
    // do nothing
    return idx;
}


void CalciumMech2::connect(compartment* comp_)
{

    // make sure the compartment has a volume defined
    if (isnan(comp_->vol)) {mexErrMsgTxt("[CalciumMech2] this mechanism requires that the volume of the compartment it is in be defined. \n");}

    comp = comp_;
}

void CalciumMech2::integrate(void)
{

    double Ca = comp->Ca_prev;

    // this convoluted method is so that we can rewrite the 
    // calcium equation in an exponential form and 
    // then use the Exponential Euler method to solve it
    // otherwise this becomes very tricky and easily 
    // diverges

    double Ca_inf = Ca_in - (tau_Ca*phi*(comp->i_Ca_prev)*(comp->A))/(192971*(comp->vol)); // microM
    comp->Ca = Ca_inf + (Ca - Ca_inf)*exp(-dt/tau_Ca);
}


double CalciumMech2::Cadot(double Ca_)
{
    return -(phi*(comp->i_Ca)*(comp->A)/(192971*(comp->vol))) - (Ca_ - Ca_in)/tau_Ca;
}

// Runge-Kutta 4 integrator 
void CalciumMech2::integrateMS(int k, double V, double Ca_)
{
    if (k == 4){return;}
    comp->k_Ca[k] = dt*(Cadot(Ca_));
}

void CalciumMech2::checkSolvers(int k)
{
    if (k == 0){
        return;
    } else if (k == 4){
        return;
    } else {
        mexErrMsgTxt("[CalciumMech1] unsupported solver order\n");
    }
}



#endif
