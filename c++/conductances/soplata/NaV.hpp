// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Fast, inactivating sodium conductance
// component source: [Soplata et al. 2017](https://www.ncbi.nlm.nih.gov/pubmed/29227992)
//
// direct link to supplementary info: https://journals.plos.org/ploscompbiol/article/file?type=supplementary&id=info:doi/10.1371/journal.pcbi.1005879.s001
// direct link to GitHub repository: https://github.com/asoplata/propofol-coupling-2017-full
//
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// (2017) Thalamocortical control of propofol phase-amplitude coupling. PLoS
// Comput Biol 13(12): e1005879. https://doi.org/10.1371/journal.pcbi.1005879
//
// dynamics were inherited from Ching et al. 2010,
// but diverge from the original publication
//
// Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N. J.
// (2010). Thalamocortical model for a propofol-induced alpha-rhythm
// associated with loss of consciousness. Proceedings of the National
// Academy of Sciences, 107(52), 22665–22670.
// http://doi.org/10.1073/pnas.1017069108

#ifndef NAV
#define NAV
#include "conductance.hpp"

//inherit conductance class spec
class NaV: public conductance {

public:

    // specify parameters + initial conditions
    NaV(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
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
