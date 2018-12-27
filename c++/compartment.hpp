// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// class defining one compartment

#ifndef COMPARTMENT
#define COMPARTMENT
#include <cmath>
#include <vector>
#include "conductance.hpp"
#include "synapse.hpp"
#include "mechanism.hpp"
class network;

#define F 96485
#define pi 3.1416

using namespace std;

class compartment
{
protected:

    vector<conductance*> cond; // pointers to all conductances in compartment
    vector<synapse*> syn; // pointers to synapses onto this neuron.
    vector<mechanism*> cont; // pointers to mechanisms
    vector<int> mechanism_sizes; // stores sizes of each mechanism's full state
    vector<int> synapse_sizes; // stores sizes of each mechanism's full state

    // vector that will store the axial synapses
    vector <synapse*> axial_syn;

    // voltage and other state variables (calcium, ..
    double sigma_g;
    double sigma_gE;
    double V_inf;

public:


    // core dynamic variables
    double V = -60;
    double Ca;
    double k_V[4] = {0,0,0,0};
    double k_Ca[4] = {0,0,0,0};


    double approx_channels = 0;
    double dt;
    double temperature;
    double temperature_ref = 11;

    // some housekeeping parameters
    // that will be useful in the
    // Crank-Nicholson integration scheme
    double c_;
    double f_;
    double delta_V;
    double i_Ca_prev = 0;
    double Ca_prev;
    double V_prev;

    double verbosity = 0;

    double RT_by_nF;

    // this int stores an integer that indicates
    // the hierarchy of this compartment in a multi-comp
    // neuron tree. tree_idx of 0 means it is a soma
    // compartment
    double tree_idx;

    // this number stores a numeric value
    // that corresponds to the neuron # of this compartment
    // in the whole network. automatically assigned
    double neuron_idx;

    // pointers to upstream and downstream compartments
    // (will be generated on initialization)
    compartment * upstream;
    compartment * downstream;

    // conductances downstream and upstream
    // will be generated on initialization
    double downstream_g;
    double upstream_g;


    // neuron parameters

    // size
    // geometry is assumed to be a cylinder.
    // if not, you must specify the area and
    // volume
    double A = .0628;
    double vol;
    double radius; // mm
    double len; // mm
    double shell_thickness; // mm


    double Cm = 10; // specific capacitance


    double Ca_target; // for homeostatic control
    double Ca_out = 3000;


    // stores the average Ca over the integration
    // window. useful for quickly determining
    // if integral control has worked without
    // pulling out the full trace
    double Ca_average;


    double E_Ca;
    double i_Ca; // specific calcium current (current/area. nA/mm^2)
    double I_ext; // all external currents are summed here
    double I_clamp; // this is the current required to clamp it
    int n_cond; // this keep tracks of the # channels
    int n_cont; // # of mechanisms
    int n_syn; // # of synapses
    int n_axial_syn;

    int solver_order = 0;

    // constructor with all parameters
    compartment(double V_, double Ca_, double Cm_, double A_, double vol_,  double Ca_target_, double Ca_average_, double tree_idx_, double neuron_idx_, double radius_, double len_, double shell_thickness_, double Ca_out_)
    {

        // core dynamic variables
        V = V_;
        Ca = Ca_;


        Ca_out = Ca_out_;

        sigma_g = 0;
        sigma_gE = 0;
        V_inf = 0;

        // geometry
        vol = vol_;
        A = A_;
        radius = radius_;
        shell_thickness = shell_thickness_;
        len = len_;

        // membrane props
        Cm = Cm_;


        // calcium
        Ca_target = Ca_target_;
        Ca_average = Ca_average_;
        Ca_average = 0; // reset it every time


        // housekeeping
        tree_idx = tree_idx_;
        neuron_idx = neuron_idx_;


        // defaults
        if (isnan (Ca_out)) { Ca_out = 3000;}
        if (isnan (A)) { A = .0628;}
        if (isnan (V)) { V = -60; } // mV
        if (isnan (Ca)) { Ca = .05; }

        // if (isnan (Ca_target)) { Ca_target = Ca_in; }

        if (!isnan(len) && !isnan(radius))
        {
            // radius and length
            // are provided, and use
            // cylindrical geometry
            // override defaults if need be
            A = 2*pi*radius*len; // + 2*pi*radius*radius;
            vol = pi*radius*radius*len;

            if (!isnan(shell_thickness))
            {
                double inner_radius = radius - shell_thickness;
                if (inner_radius > 0)
                {
                    vol = pi*len*(radius*radius - inner_radius*inner_radius);
                }
            }
        }


        // housekeeping
        E_Ca = 0; // because this will be computed from the Nernst eq. 
        i_Ca = 0; // this is the current density (nA/mm^2)
        n_cond = 0;
        n_cont = 0;
        n_syn = 0;
        n_axial_syn = 0;
        upstream = NULL;
        downstream = NULL;
        upstream_g = 0;
        downstream_g = 0;

        f_ = 0;
        c_ = 0;
        delta_V = 0;

    }

    // begin function declarations

    // methods to add things to compartment
    void addConductance(conductance*);
    void addSynapse(synapse*);
    void addAxial(synapse*);
    void addMechanism(mechanism*);

    // integration methods
    void integrateMS(int);
    void integrateMechanisms(void);
    void integrateChannels(void);
    void integrateSynapses(void);
    void integrateVoltage(void);
    void integrateV_clamp(double);

    // methods for integrating using Crank-Nicholson
    // and methods for multi-compartment models
    double getBCDF(int);
    void integrateCNFirstPass(void);
    void integrateCNSecondPass(void);
    void resolveAxialConductances(void);

    // methods to retrieve information from compartment
    int getFullMechanismState(double*, int);
    int getFullCurrentState(double*, int);
    int getFullSynapseState(double*, int);
    int getFullMechanismSize(void);
    int getFullSynapseSize(void);

    mechanism* getMechanismPointer(int);
    mechanism* getMechanismPointer(const char*); // overloaded
    compartment* getConnectedCompartment(int);
    conductance* getConductancePointer(const char*);
    conductance* getConductancePointer(int);

    synapse* getSynapsePointer(int);
    

    void checkSolvers(int);


};

// methods are arranged alphabetically

// add axial to this compartment
void compartment::addAxial(synapse *syn_)
{
    axial_syn.push_back(syn_);
    n_axial_syn++;
}

// add conductance and provide pointer back to compartment
void compartment::addConductance(conductance *cond_)
{
    cond.push_back(cond_);
    cond_->verbosity = verbosity;
    cond_->buildLUT(approx_channels);
    cond_->dt = dt;
    cond_->temperature_ref = temperature_ref;
    cond_->temperature = temperature;
    cond_->connect(this);

    n_cond++;

    if (verbosity > 0)
    {
        mexPrintf("[C++] adding conductance of type: %s\n", cond_->getClass().c_str());
    }
}

// add mechanism to this compartment
void compartment::addMechanism(mechanism *mech_)
{
    // mexPrintf("adding mechanism @  %p\n",mech_);
    mech_->dt = dt;
    // mech_->verbosity = verbosity;
    mech_->temperature = temperature;
    mech_->temperature_ref = temperature_ref;
    cont.push_back(mech_);
    mech_->mechanism_idx = n_cont; // tell the mechanism what rank it has
    n_cont++;

    // also store the mechanism's full state size
    mechanism_sizes.push_back(mech_->getFullStateSize());

}

// add synapse to this compartment (this compartment is after synapse)
void compartment::addSynapse(synapse *syn_) {
    syn_->dt = dt;
    syn_->verbosity = verbosity;
    syn.push_back(syn_);
    n_syn ++;

    // also store the synapse's full state size
    synapse_sizes.push_back(syn_->getFullStateSize());
}


void compartment::checkSolvers(int solver_order) {

    if (solver_order == 0){
        return;
    } else if (solver_order == 4){
        for (int i=0; i<n_cond; i++) {
            cond[i]->checkSolvers(solver_order);
        }

        for (int i=0; i<n_cont; i++) {
            cont[i]->checkSolvers(solver_order);
        }

        for (int i=0; i<n_syn; i++) {
            syn[i]->checkSolvers(solver_order);
        }

    } else {
        mexErrMsgTxt("[compartment] Unsupported solver order \n");
    }
}

conductance* compartment::getConductancePointer(const char* cond_class) {
    conductance* req_cond = NULL;

    for (int i = 0; i < n_cond; i ++)
    {
        if ((cond[i]->getClass()) == cond_class)
        {
            req_cond = cond[i];
        }
    }

    return req_cond;
}

synapse * compartment::getSynapsePointer(int syn_idx){
    if (syn_idx < n_syn) { return syn[syn_idx];} 
    else { return NULL; } 
}

conductance * compartment::getConductancePointer(int cond_idx){
    if (cond_idx < n_cond) { return cond[cond_idx];} 
    else { return NULL; } 
}


mechanism * compartment::getMechanismPointer(int mech_idx){
    if (mech_idx < n_cont) { return cont[mech_idx];} 
    else { return NULL; }   
}

mechanism* compartment::getMechanismPointer(const char* cond_class){
    mechanism* req_cont = NULL;

    for (int i = 0; i < n_cont; i ++)
    {
        // mexPrintf("this class = %s\n", (cont[i]->controlling_class).c_str());
        if ((cont[i]->controlling_class) == cond_class)
        {
            req_cont = cont[i];
        }
    }

    return req_cont;
}


// returns a list of connected compartments.
// this function returns only one pointer at a time
// to get them all, iterative over the integer argument
compartment* compartment::getConnectedCompartment(int idx) {
    compartment* neighbour = NULL;
    if (idx > n_axial_syn) {return neighbour;}
    neighbour = axial_syn[idx]->pre_syn;
    return neighbour;
}

// returns the size of data frame for all synapses 
// in this compartment
// includes synaptic currents interleaved
// with state variables of each synapse
int compartment::getFullSynapseSize(void) {
    int full_size = 0;
    for (int i=0; i<n_syn; i++)
    {
        full_size += syn[i]->getFullStateSize();
    }
    return full_size;
}


int compartment::getFullMechanismSize(void) {
    int full_size = 0;
    for (int i=0; i<n_cont; i++)
    {
        full_size += cont[i]->getFullStateSize();
    }
    return full_size;
}


// helper function used in the Crank-Nicholson scheme
// and returns B, C, D and F values as defined in eq.
// 6.45 of Dayan and Abbott
double compartment::getBCDF(int idx){
    if (idx == 0) {
        return 0;
    } else if (idx == 1) {
        // return B
        if (upstream){
            return (upstream_g/Cm);

        } else {
            // no upstream, 1st compartment,
            // return 0
            return 0;
        }

    } else if (idx == 2) {
        // return C
        double C = sigma_g;
        if (downstream) {C += downstream_g;}
        if (upstream) {C += upstream_g;}
        C = -C/Cm;
        return C;

    } else if (idx == 3) {
        // return D
        if (downstream) {
            return (downstream_g/Cm);
        } else {
            // no downstream
            return 0;
        }

    } else if (idx == 4) {
        // return F
       return (sigma_gE + I_ext/A)/Cm;

    } else  {
        return 0;
    }

}


void compartment::integrateMS(int k){


    if (k == 4) {
        // terminal calculations, advance step
        V = V_prev + (k_V[0] + 2*k_V[1] + 2*k_V[2] + k_V[3])/6;
        Ca = Ca_prev + (k_Ca[0] + 2*k_Ca[1] + 2*k_Ca[2] + k_Ca[3])/6;
    }


    // reset some things
    sigma_g = 0;
    sigma_gE = 0;
    i_Ca = 0;

    double V_MS;
    double Ca_MS;

    if (k == 0) {
        // first step
        V_MS = V_prev;
        Ca_MS = Ca_prev;
    } else if (k == 1) {
        V_MS = V_prev + k_V[0]/2;
        Ca_MS = Ca_prev + k_Ca[0]/2;
    } else if (k == 2) {
        V_MS = V_prev + k_V[1]/2;
        Ca_MS = Ca_prev + k_Ca[1]/2;
    } else if (k == 3) {
        V_MS = V_prev + k_V[2];
        Ca_MS = Ca_prev + k_Ca[2];
    }

    E_Ca = RT_by_nF*log((Ca_out)/(Ca_MS));


    // channels
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->integrateMS(k, V_MS, Ca_MS);
        sigma_g += cond[i]->g;
        sigma_gE += (cond[i]->g)*(cond[i]->E);
    }

    //synapses 
    for (int i=0; i<n_syn; i++) {
        syn[i]->integrateMS(k, V_MS, Ca_MS);
        sigma_g += (syn[i]->gmax)*(syn[i]->s)/(1000*A); // now uS/mm^2
        sigma_gE += ((syn[i]->gmax)*(syn[i]->s)*(syn[i]->E)/(1000*A));
    }


    // mechanisms
    for (int i=0; i<n_cont; i++) {
        cont[i]->integrateMS(k, V_MS, Ca_MS);
    }


    //voltage 
    // only compute when k < 4
    if (k == 4) {return;}
    

    k_V[k] = dt*(sigma_gE - sigma_g*V_MS)/Cm;
    
    // mexPrintf("V_MS =  %f\n", V_MS);

}


void compartment::integrateChannels(void) {

    sigma_g = 0.0;
    sigma_gE = 0.0;


    // compute E_Ca
    E_Ca = RT_by_nF*log((Ca_out)/(Ca_prev));

    // integrate all channels
    for (int i=0; i<n_cond; i++)
    {
        cond[i]->integrate(V_prev, Ca_prev);
        sigma_g += cond[i]->g;
        sigma_gE += (cond[i]->g)*(cond[i]->E);
    }

    // update the running total Ca
    Ca_average += Ca;
}



void compartment::integrateMechanisms(void) {
    for (int i=0; i<n_cont; i++) {
        cont[i]->integrate();
    }
}


void compartment::integrateSynapses(void) {
    // we treat synapses identically to any other conductance
    for (int i=0; i<n_syn; i++) {
        syn[i]->integrate();
        sigma_g += (syn[i]->g)/(1000*A); // now uS/mm^2
        sigma_gE += ((syn[i]->g)*(syn[i]->E))/(1000*A); // now uS/mm^2
    }
}


void compartment::integrateVoltage(void) {

    // compute infinity values for V and Ca
    if (sigma_g == 0) {
        V_inf = V_prev;
    }
    else {
        V_inf = (sigma_gE + (I_ext/A))/sigma_g;
    }

    // integrate V 
    V = V_inf + (V_prev - V_inf)*exp(-dt/(Cm/(sigma_g)));

}


// assumes the cell is being clamped, and 
// integrates and solves for I_clamp

void compartment::integrateV_clamp(double V_clamp) {

    // calculate I_clamp, and set voltage to the clamped
    // voltage
    double E = exp(-dt/(Cm/(sigma_g)));
    V_inf = (V_clamp - V*E)/(1 - E);
    //mexPrintf("sigma_g=  %f\n",sigma_g);
    I_clamp =  A*(V_inf*sigma_g - sigma_gE);

    V = V_clamp;

}


void compartment::integrateCNFirstPass(void) {
    // intermiediate variables
    double b; // b is b for this compartment
    double d; // d is d for the prev compartment

    b = getBCDF(1)*.5*dt;


    // compute c_
    c_ = .5*dt*getBCDF(2);
    if (upstream) {

        d = (upstream->getBCDF(3))*dt*.5;

        // full expression for c_ (eq. 6.54)
        c_ += b*d/(1 - upstream->c_);

    }

    // compute f_
    // first compute f
    double f = getBCDF(4) + getBCDF(2)*V;
    if (upstream) {
        f += getBCDF(1)*(upstream->V);
    }
    if (downstream) {
        f += getBCDF(3)*(downstream->V);
    }
    f = f*dt;

    f_ = f;

    if (upstream) {
        // downstream exists. append terms
        // (eq. 6.55 in Dayan & Abbott)
        f_ += (b*(upstream->f_))/(1 - upstream->c_);
    }

}

void compartment::integrateCNSecondPass(void) {
    delta_V = f_;
    if (downstream) {
        // downstream exists, use full eq (6.53)
        delta_V += getBCDF(3)*.5*dt*(downstream->delta_V);
    }

    // divide by common denominator
    delta_V = delta_V/(1 - c_);


    V += delta_V;
}

// returns a vector of the state of every mechanism
// mech_state is a pointer to a matrix that is hopefully of
// the right size
int compartment::getFullMechanismState(double *mech_state, int idx)
{
    for (int i = 0; i < n_cont; i ++)
    {

        cont[i]->getFullState(mech_state, idx);
        idx += mechanism_sizes[i];

    }
    return idx;
}

// for ionic currents
int compartment::getFullCurrentState(double *cond_state, int idx)
{
    for (int i = 0; i < n_cond; i ++)
    {
        cond_state[idx] = cond[i]->getCurrent(V);
        idx ++;
    }
    return idx;
}

// for synaptic currents
int compartment::getFullSynapseState(double *syn_state, int idx)
{
    for (int i = 0; i < n_syn; i ++)
    {
        idx = syn[i]->getFullState(syn_state,idx);
    }
    return idx;
}


void compartment::resolveAxialConductances(void)
{

    if (n_axial_syn == 0) {return;}

    for (int i = 0; i < n_axial_syn; i++)
    {
        if (isnan((axial_syn[i]->pre_syn)->tree_idx)) { continue;}

        if ((axial_syn[i]->pre_syn)->tree_idx > tree_idx)
        {
            // pre_syn of this axial synapse is downstream
            downstream_g = axial_syn[i]->gmax;
        } else {
            // pre_syn of this axial synapse is upstream
            upstream_g = axial_syn[i]->gmax;
        }
    }

    // debug info
    // mexPrintf("===========\n");
    // mexPrintf("tree_idx of this compartment: %f\n",tree_idx);
    // mexPrintf("downstream_g = %f\n",downstream_g);
    // mexPrintf("upstream_g = %f\n",upstream_g);

}


#endif
