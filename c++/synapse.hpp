/*

This document describes the "synapse" C++ class.
This class describes objects that are synapses which are
conductances in channels whose activity depends on the
activity of another neuron. 

| Abstract | can contain | contained in |
| --------  | ------ | -------  |
| yes |  nothing | compartment |



*/
#ifndef SYNAPSE
#define SYNAPSE
#include <cmath>
class compartment;

class synapse {
protected:


    
public:
    compartment *post_syn; // pointer to pre-synaptic compartment
    compartment *pre_syn; // pointer to pre-synaptic compartment
    double gmax = 0;
    double g = 0;
    double E = 0;
    
    // dynamic variables
    double s = 0;
    double k_s[4] = {0,0,0,0};

    bool is_electrical = false;

    double dt = 0;
    double verbosity = 0;
    double temperature_ref = 11;
    double temperature = 11;


    synapse()
    {
        pre_syn = 0; //null pointer for safety
        post_syn = 0;
    }
    ~synapse() {}

    virtual void integrate(void);
    virtual void integrateMS(int, double, double);

    virtual void checkSolvers(int);

    virtual void connect(compartment*, compartment*) = 0;// tells compiler they will be overridden by derived class
    virtual int getFullStateSize(void) = 0;
    virtual int getFullState(double*, int) = 0;


};

/*
This method is used to integrate the synapse and update 
its dynamical variables. Since synapses can be arbitrary dynamical
systems, this method is virtual, and needs to be overridden
by a concrete integration scheme defined in a particular 
synapse type.
*/
void synapse::integrate() {
    mexErrMsgTxt("[synapse] Unimplemented integration method\n");
}

/*
This method is used to integrate the synapse and update 
its dynamical variables when a multi-step solver is requestred. 
Since synapses can be arbitrary dynamical systems, this method 
is virtual, and needs to be overridden
by a concrete integration scheme defined in a particular 
synapse type.
*/
void synapse::integrateMS(int k, double V, double Ca) {
    mexErrMsgTxt("[synapse] Unimplemented integration method\n");
}


/*
This method checks whether the synapse can use the requested
solver order to integrate its equations. If it can't, it 
should throw an error. 
*/
void synapse::checkSolvers(int k){
    mexErrMsgTxt("[synapse] Unsupported solver order\n");
}


#endif
