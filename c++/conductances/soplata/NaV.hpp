// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// fast sodium conductance (inactivating)
// Soplata et al. 2017
// See also:
// Kramer, M. A., Roopun, A. K., Carracedo, L. M., Traub, R. D., Whittington, M. A., & Kopell, N. J. (2008). Rhythm generation through period concatenation in rat somatosensory cortex. PLoS computational biology, 4(9), e1000169. (http://www.ploscompbiol.org/article/info%3Adoi%2F10.1371%2Fjournal.pcbi.1000169)
// Traub RD, Buhl EH, Gloveli T, Whittington MA (2003) Fast rhythmic bursting can be induced in layer 2/3 cortical neurons by enhancing persistent Na+ conductance or by blocking BK channels. J Neurophysiol 89:909–921.
// Cunningham MO, Whittington MA, Bibbig A, Roopun A, LeBeau FEN, et al. (2004) A role for fast rhythmic bursting neurons in cortical gamma oscillations in vitro. Proc Natl Acad Sci USA 101:7152–7157.

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
        if (isnan (E)) { E = 50; }

        p = 3;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string NaV::getClass(){return "NaV";}


double NaV::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double NaV::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double NaV::tau_m(double V, double Ca) {return 1.32 - 1.26/(1+exp((V+120.0)/-25.0));}
double NaV::tau_h(double V, double Ca) {return (0.67/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6)));}


#endif
