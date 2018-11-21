// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// T-type Calcium current for generic thalamocortical relay cells
// Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N.
// J. (2010). Thalamocortical model for a propofol-induced alpha-rhythm
// associated with loss of consciousness. Proceedings of the National Academy of
// Sciences, 107(52), 22665-22670. doi:10.1073/pnas.1017069108

#ifndef CAT
#define CAT
#include "conductance.hpp"

//inherit conductance class spec
class CaT: public conductance {

public:

    // specify parameters + initial conditions
    CaT(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 0; }
        if (isnan (E)) { E = 30; }
    }

    void integrate(double, double);

    double m_inf(double, double);
    double tau_m(double, double);
    double h_inf(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string CaT::getClass(){return "CaT";}

void CaT::integrate(double V, double Ca)
{
    // update E by copying E_Ca from the cell
    E = container->E_Ca;

    m = m_inf(V,Ca) + (m - m_inf(V,Ca))*exp(-dt/tau_m(V,Ca));
    h = h_inf(V,Ca) + (h - h_inf(V,Ca))*exp(-dt/tau_h(V,Ca));
    g = gbar*m*m*m*h;

    // compute the specific calcium current and update it in the cell
    double this_I = g*(V-E);
    container->i_Ca += this_I;
}

double CaT::m_inf(double V, double Ca) {return 1./(1.+exp(-(V+4.+50.0)/7.4));}
double CaT::h_inf(double V, double Ca) {return 1./(1.+exp((V+4.+78.0)/5));}
double CaT::tau_m(double V, double Ca) {return (3. + (1.0/(exp((V+4.+25.0)/10.)+exp(-(V+4.+100.0)/15.0))))/6.81;}
double CaT::tau_h(double V, double Ca) {return (85.0 + (1.0/(exp((V+4.+46.0)/4.)+exp(-(V+4.+405.0)/50.0))))/3.73;}

// NOTE:
// phiM = 6.81;  % unitless, based on Q10 of 5
// phiH = 3.73;  % unitless, based on Q10 of 3

#endif
