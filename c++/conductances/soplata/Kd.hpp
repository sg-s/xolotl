// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// delayed rectifier potassium conductance
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879
// Kramer, M. A., Roopun, A. K., Carracedo, L. M., Traub, R. D., Whittington, M. A., & Kopell, N. J. (2008). Rhythm generation through period concatenation in rat somatosensory cortex. PLoS computational biology, 4(9), e1000169. (http://www.ploscompbiol.org/article/info%3Adoi%2F10.1371%2Fjournal.pcbi.1000169)
// Traub RD, Buhl EH, Gloveli T, Whittington MA (2003) Fast rhythmic bursting can be induced in layer 2/3 cortical neurons by enhancing persistent Na+ conductance or by blocking BK channels. J Neurophysiol 89:909–921.
// Cunningham MO, Whittington MA, Bibbig A, Roopun A, LeBeau FEN, et al. (2004) A role for fast rhythmic bursting neurons in cortical gamma oscillations in vitro. Proc Natl Acad Sci USA 101:7152–7157.


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
        if (isnan (E)) { E = -95; }

        p = 4;

        // allow this channel to be approximated
        approx_m = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

};

string Kd::getClass(){return "Kd";}

double Kd::m_inf(double V, double Ca) {return 1./(1+exp((-V-29.5)/10));}
double Kd::tau_m(double V, double Ca) {return .25+4.35*exp(-abs(V+10)/10);}

#endif
