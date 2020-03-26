// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Delayed rectifier potassium current
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
        if (isnan (E)) { E = -100; }

        p = 4;

        // allow this channel to be approximated
        approx_m = 1;

    }

    double m_alpha(double, double);
    double m_beta(double, double);

    double m_inf(double, double);
    double tau_m(double, double);

    string getClass(void);

};

string Kd::getClass(){return "Kd";}

double Kd::m_alpha(double V, double Ca) {return 0.032 * (15 - (V + 25)) / (exp((15 - (V + 25))/5) -1);}
double Kd::m_beta(double V, double Ca) {return 0.5 * exp((10 - (V + 25))/40);}

double Kd::m_inf(double V, double Ca) {return m_alpha(V, Ca) / (m_alpha(V, Ca) + m_beta(V, Ca));}
double Kd::tau_m(double V, double Ca) {return 1 / (m_alpha(V, Ca) + m_beta(V, Ca));}

#endif
