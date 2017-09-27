//Abstract class for defining synapses  
#ifndef SYNAPSE
#define SYNAPSE
#include <cmath>
class compartment;

class synapse {
protected:
    // parameters
    double Delta;
    
    double k_;
    double Vth;


    compartment *pre_syn; // pointer to pre-synaptic compartment 
    compartment *post_syn; // pointer to pre-synaptic compartment 
public:

    double s;
    double gbar;
    double E;
    

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
