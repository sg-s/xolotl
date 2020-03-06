// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Ca++ dependent nonspecific cation current
// https://github.com/asoplata/propofol-coupling-2017-full/blob/master/dynasim/models/iCan.mech
// Soplata et al. 2017

#ifndef CAN
#define CAN
#include "conductance.hpp"

//inherit conductance class spec
class CaN: public conductance {

protected:
    double alpha = 0.0056;
    double beta = 0.002;

public:

    //specify both gbar and erev and initial conditions
    CaN(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -20; }

        p = 2;

        // allow this channel to be approximated
        approx_m = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string CaN::getClass(){return "CaN";}

double CaN::m_inf(double V, double Ca) { return alpha * Ca / (alpha * Ca + beta); }
double CaN::tau_m(double V, double Ca) { return 1.0 / (alpha * Ca + beta); }

#endif
