// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Deleyed rectifier
// Dethier, Drion, Franci, Sepulchre 2015
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4620135/

#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

public:

    // specify parameters + initial conditions
    Kd(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = 30; }


        p = 4;
        approx_m = 1;
    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);


};

string Kd::getClass(){return "Kd";}

double Kd::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+12.3)/-11.8)));}
double Kd::tau_m(double V, double Ca) {return 7.2- (6.4/(1.0+exp(((V)+28.3)/-19.2)));}

#endif
