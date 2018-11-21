// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// fast sodium current for generic thalamic reticular cells
// Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N.
// J. (2010). Thalamocortical model for a propofol-induced alpha-rhythm
// associated with loss of consciousness. Proceedings of the National Academy of
// Sciences, 107(52), 22665-22670. doi:10.1073/pnas.1017069108


#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions
    NaV(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 0; }
        if (isnan (E)) { E = 50; }
    }

    void integrate(double, double);

    double m_a(double, double);
    double m_b(double, double);
    double h_a(double, double);
    double h_b(double, double);

    double m_inf(double, double);
    double tau_m(double, double);
    double h_inf(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string NaV::getClass(){return "NaV";}

void NaV::integrate(double V, double Ca)
{

    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*h;

}

double NaV::m_a(double V, double Ca) {return (.32*(13.-(V+55.)))/(exp((13.-(V+55.))/4.)-1.);}
double NaV::m_b(double V, double Ca) {return (.28*((V+55.)-40.))/(exp(((V+55.)-40.)/5.)-1.);}
double NaV::h_a(double V, double Ca) {return .128*exp((17-(V+55.))/18.);}
double NaV::h_b(double V, double Ca) {return 4./(1.+exp((40.-(V+55.))/5.));}

double NaV::m_inf(double V, double Ca) {return m_a(V, Ca) / (m_a(V,Ca) + m_b(V,Ca));}
double NaV::h_inf(double V, double Ca) {return h_a(V, Ca) / (h_a(V,Ca) + h_b(V,Ca));}
double NaV::tau_m(double V, double Ca) {return 1.0;}
double NaV::tau_h(double V, double Ca) {return 1.0;}

#endif
