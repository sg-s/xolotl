// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// GenericMH inactivating conductance
// defaults reproduce a Sodium channel from here:
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef GENERICNAVLIKE
#define GENERICNAVLIKE
#include "conductance.hpp"

//inherit conductance class spec
class GenericNaVLike: public conductance {

public:


    double m_V_half = -25.5;
    double m_V_slope = -5.29;

    double h_V_half = -48.9;
    double h_V_slope = 5.18;

    double m_tau_A = 1.32;
    double m_tau_B = -1.26;
    double m_tau_V_half = -120.0;
    double m_tau_V_slope = -25.0;


    double h_tau_A1 = 0;
    double h_tau_B1 = .67;
    double h_tau_V_half1 = -62.9;
    double h_tau_V_slope1 = -10.0;

    double h_tau_A2 = 1.5;
    double h_tau_B2 = 1;
    double h_tau_V_half2 = -34.9;
    double h_tau_V_slope2 = 3.6;

    // specify parameters + initial conditions
    GenericNaVLike(double g_, double E_, double m_, double h_, double m_V_half_, double m_V_slope_, double h_V_half_, double h_V_slope_, double m_tau_A_, double m_tau_B_, double m_tau_V_half_, double m_tau_V_slope_, double h_tau_A1_,double h_tau_B1_, double h_tau_V_half1_, double h_tau_V_slope1_, double h_tau_A2_, double h_tau_B2_, double h_tau_V_half2_, double h_tau_V_slope2_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;

        // activation parameters
        m_V_half = m_V_half_;
        m_V_slope = m_V_slope_;

        h_V_half = h_V_half_;
        h_V_slope = h_V_slope_;

        m_tau_A = m_tau_A_;
        m_tau_B = m_tau_B_;
        m_tau_V_half = m_tau_V_half_;
        m_tau_V_slope = m_tau_V_slope_;

        h_tau_A1 = h_tau_A1_;
        h_tau_B1 = h_tau_B1_;
        h_tau_V_half1 = h_tau_V_half1_;
        h_tau_V_slope1 = h_tau_V_slope1_;

        h_tau_A2 = h_tau_A2_;
        h_tau_B2 = h_tau_B2_;
        h_tau_V_half2 = h_tau_V_half2_;
        h_tau_V_slope2 = h_tau_V_slope2_;




        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = 30; }


        p = 3;
        q = 1;

        // do not allow this channel to be approximated
        approx_m = 0;
        approx_h = 0;
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);
};

string GenericNaVLike::getClass(){return "GenericNaVLike";}


double GenericNaVLike::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V-m_V_half)/m_V_slope));}
double GenericNaVLike::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V-h_V_half)/h_V_slope));}
double GenericNaVLike::tau_m(double V, double Ca) {return m_tau_A + m_tau_B/(1+exp((V - m_tau_V_half)/m_tau_V_slope));}

double GenericNaVLike::tau_h(double V, double Ca) {return (h_tau_A1 + h_tau_B1/(1.0+exp((V - h_tau_V_half1)/h_tau_V_slope1)))*(h_tau_A2+h_tau_B2/(1.0+exp((V-h_tau_V_half2)/h_tau_V_slope2)));}


#endif
