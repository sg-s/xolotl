// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// fast potassium current for generic thalamocortical relay cells
// Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N.
// J. (2010). Thalamocortical model for a propofol-induced alpha-rhythm
// associated with loss of consciousness. Proceedings of the National Academy of
// Sciences, 107(52), 22665-22670. doi:10.1073/pnas.1017069108


#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

public:

    // specify parameters + initial conditions
    Kd(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 0; }
        if (isnan (E)) { E = -80; }
    }

    void integrate(double, double);

    double m_a(double, double);
    double m_b(double, double);

    double m_inf(double, double);
    double tau_m(double, double);
    double h_inf(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string Kd::getClass(){return "Kd";}

void Kd::integrate(double V, double Ca)
{

    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*m;

}

double Kd::m_a(double V, double Ca) {return .032*(15.-(X+25.))/(exp((15.-(X+25.))/5.)-1.);}
double Kd::m_b(double V, double Ca) {return .5*exp((10.-(X+25.))/40.);}

double Kd::m_inf(double V, double Ca) {return m_a(V, Ca) / (m_a(V,Ca) + m_b(V,Ca));}
double Kd::h_inf(double V, double Ca) {return 1.0;}
double Kd::tau_m(double V, double Ca) {return 1.0;}
double Kd::tau_h(double V, double Ca) {return 1.0;}

#endif
