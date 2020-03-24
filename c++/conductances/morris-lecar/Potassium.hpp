// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// generic conductance template
// you can either fill this out yourself
// or use the conductance class within MATLAB
// to auto-generate C++ code 

#ifndef POTASSIUM
#define POTASSIUM
#include "conductance.hpp"

//inherit conductance class spec
class Potassium: public conductance {

public:

    double V3 = 0;
    double V4 = 1;
    double phi = .04;

    // specify parameters + initial conditions
    Potassium(double gbar_, double E_, double m_, double V3_, double V4_, double phi_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        phi = phi_;
        V3 = V3_;
        V4 = V4_;

        // defaults 
        if (isnan(gbar)) { gbar = 8; }
        if (isnan (m)) { m = 0; }
        if (isnan (V3)) { m = 30; }
        if (isnan (V4)) { m = 2; }
        if (isnan (E)) { E = -84; }

        p = 1;
        q = 0;
    }

    double m_inf(double V, double Ca);
    double tau_m(double V, double Ca);

    string getClass(void);


};

string Potassium::getClass(){return "Potassium";}



double Potassium::m_inf(double V, double Ca) {return  (.5)*(1 + tanh((V-V3)/(V4)));}
double Potassium::tau_m(double V, double Ca) {return  1/(phi*cosh((V-V3)/(2*V4)));}


#endif

