// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// abstract class for defining synapses
// synapses connect two compartments
// typically, neurons.

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



    compartment *post_syn; // pointer to pre-synaptic compartment
public:

    compartment *pre_syn; // pointer to pre-synaptic compartment
    double s;
    double gbar;
    double E;
    bool is_electrical;


    synapse()
    {
        pre_syn = 0; //null pointer for safety
        post_syn = 0;
    }
    ~synapse() {}

    virtual void integrate(double) = 0;
    virtual void connect(compartment*, compartment*) = 0;// tells compiler they will be overridden by derived class
    virtual int getFullStateSize(void) = 0;
    virtual int getFullState(double*, int) = 0;


};


#endif
