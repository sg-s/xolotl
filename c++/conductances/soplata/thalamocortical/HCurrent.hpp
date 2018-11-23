// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// hyperpolarization-activated inward rectifying mixed cation current
//
// "Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N.
// J. (2010). Thalamocortical model for a propofol-induced alpha-rhythm
// associated with loss of consciousness. Proceedings of the National Academy of
// Sciences, 107(52), 22665-22670. doi:10.1073/pnas.1017069108"

#ifndef HCURRENT
#define HCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {

public:

    //specify both gbar and erev and initial conditions
    HCurrent(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -20; }

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);


};

string HCurrent::getClass(){
    return "HCurrent";
}

double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+70.0)/6.0));}
double HCurrent::tau_m(double V, double Ca) {return (272.0 + 1499.0/(1.0+exp((V+42.2)/-8.73)));}


#endif
