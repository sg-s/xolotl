// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component source [Morris & Lecar]()
// component info: Morris & Lecar Calcium conductance 

#ifndef CACURRENT
#define CACURRENT
#include "conductance.hpp"

//inherit conductance class spec
class CaCurrent: public conductance {

public:

    double V1 = 0;
    double V2 = 1;
    double E = 100;


    // specify parameters + initial conditions
    CaCurrent(double gbar_, double V1_, double V2_, double E_)
    {
        gbar = gbar_;
        V1 = V1_;
        V2 = V2_;
        E = E_;

        // defaults 
        if (isnan(gbar)) { gbar = 4.4; }
        if (isnan(E)) { E = 120; }
        if (isnan (V1)) { V1 = -1.2; }
        if (isnan (V2)) { V2 = 18; }

        p = 1;
        q = 0;
    }

    void integrate(double, double);

    double m_inf(double V, double CaCurrent);
    string getClass(void);


};

string CaCurrent::getClass(){return "CaCurrent";}

void CaCurrent::integrate(double V, double Ca) {

    if (isnan(E)) {
        // update form container
        E = container->E_Ca;
    } else {
        // update to container
        container->E_Ca = E;
    }

    g = gbar*m_inf(V,Ca);

    container->i_Ca += getCurrent(V);
    
}


double CaCurrent::m_inf(double V, double Ca) {return  0.5*(1.0 + tanh((V-V1)/(V2)));}



#endif

