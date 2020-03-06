// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// inward rectifying potassium conductance
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef KD2
#define KD2
#include "conductance.hpp"

//inherit conductance class spec
class Kd2: public conductance {

public:

    // true reversal potential
    // the E here is set to zero so we can linearize the equation
    double Erev = -80.0; 

    //specify both gbar and erev and initial conditions
    Kd2(double gbar_, double E_, double Erev_)
    {
        gbar = gbar_;
        Erev = Erev_;


        // not used
        E = E_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -80; }

        p = 4;

        // allow this channel to be approximated
        approx_m = 0;

        // need to set E to zero to linearize. Don't worry,
        // this is not the reversal potential
        E = 0;
    }

    string getClass(void);

    void integrate(double, double);


};

string Kd2::getClass(){
    return "Kd2";
}


void Kd2::integrate(double V_, double Ca_){


    // Equation 1  can be re-written to be in the form
    // I = gV - gE
    // where E = 0. Then, g is given by
    // a dynamically changing variable 

    // Note that equation 1 is wrong -- it's not qV-EK, it's
    // q(V-EK)
    g = gbar*(exp((V_-Erev)/25) - 1)/(exp(V_/25) - 1);

}



#endif


