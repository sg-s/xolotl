// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Generic non-inactivating conductance
// defaults reproduce a Kd channel from here:
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef GENERICKDLIKE
#define GENERICKDLIKE
#include "conductance.hpp"

//inherit conductance class spec
class GenericKdLike: public conductance {

public:


    double m_V_half = -12.3;
    double m_V_slope = -11.8;

    double m_tau_A = 7.2;
    double m_tau_B = -6.4;
    double m_tau_V_half = -28.30;
    double m_tau_V_slope = -19.2;


    // specify parameters + initial conditions
    GenericKdLike(double gbar_, double E_, double m_, double h_, double m_V_half_, double m_V_slope_, double m_tau_A_, double m_tau_B_, double m_tau_V_half_, double m_tau_V_slope_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // activation parameters
        m_V_half = m_V_half_;
        m_V_slope = m_V_slope_;


        m_tau_A = m_tau_A_;
        m_tau_B = m_tau_B_;
        m_tau_V_half = m_tau_V_half_;
        m_tau_V_slope = m_tau_V_slope_;



        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }


        p = 4;

        // do not allow this channel to be approximated
        approx_m = 0;
    }

    double m_inf(double, double);
    double tau_m(double, double);
    string getClass(void);
};

string GenericKdLike::getClass(){return "GenericKdLike";}


double GenericKdLike::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V-m_V_half)/m_V_slope));}

double GenericKdLike::tau_m(double V, double Ca) {return m_tau_A + m_tau_B/(1+exp((V - m_tau_V_half)/m_tau_V_slope));}



#endif
