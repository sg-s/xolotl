//Abstract class for defining synapses  
#ifndef SYNAPSE
#define SYNAPSE
#include <cmath>
class compartment;

class synapse {
protected:
    // parameters
    double gbar;
    double Delta;
    double E;
    double k_;
    double Vth;

    // dynamic variables
    double s;

    compartment *pre_syn; // pointer to pre-synaptic compartment 
    compartment *post_syn; // pointer to pre-synaptic compartment 
public:
    

    synapse()
    {
        pre_syn = 0; //null pointer for safety
        post_syn = 0;
    }
    ~synapse() {}
    
    virtual void integrate(double) = 0;
    virtual void connect(compartment*, compartment*) = 0;// tells compiler they will be overridden by derived class
    virtual double getCurrent(double) = 0; 

};
#endif
