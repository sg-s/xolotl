// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// slow potassium (muscarinic-inactivated) current
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// Thalamocortical control of propofol phase-amplitude coupling. PLOS
// Computational Biology. 2017;13: e1005879. doi:10.1371/journal.pcbi.1005879
// https://raw.githubusercontent.com/asoplata/propofol-coupling-2017-full/master/dynasim/models/iM.mech
//
// % Kramer, M. A., Roopun, A. K., Carracedo, L. M., Traub, R. D., Whittington, M. A., & Kopell, N. J. (2008). Rhythm generation through period concatenation in rat somatosensory cortex. PLoS computational biology, 4(9), e1000169. (http://www.ploscompbiol.org/article/info%3Adoi%2F10.1371%2Fjournal.pcbi.1000169)
// % Traub RD, Buhl EH, Gloveli T, Whittington MA (2003) Fast rhythmic bursting can be induced in layer 2/3 cortical neurons by enhancing persistent Na+ conductance or by blocking BK channels. J Neurophysiol 89:909–921.

#ifndef MCURRENT
#define MCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class MCurrent: public conductance {

public:

    // specify parameters + initial conditions
    MCurrent(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = -95; }

        p = 1;

        // allow this channel to be approximated
        approx_m = 1;

    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);

    double alpha(double, double);
    double beta(double, double);

};

string MCurrent::getClass(){return "MCurrent";}

double MCurrent::alpha(double V, double Ca) { return .02/(1+exp((-20-V)/5)); }
double MCurrent::beta(double V, double Ca) { return .01*exp((-43-V)/18); }

double MCurrent::m_inf(double V, double Ca) {return alpha(V, Ca) / (alpha(V, Ca) + beta(V, Ca));}
double MCurrent::tau_m(double V, double Ca) {return 1.0 / (alpha(V, Ca) + beta(V, Ca));}

#endif
