// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Generic conductance
// from Nadim 1998 
// "Frequency Regulation of a Slow Rhythm by a Fast Periodic Input"
// 

#ifndef GENERICCHANNEL
#define GENERICCHANNEL
#include "conductance.hpp"

//inherit conductance class spec
class GenericChannel: public conductance {

public:


    // parameters as in paper
    // I've added h and m suffixes because
    // those parameters are different for activation
    // and inactivation gates
    double kh;
    double lh;
    double Vlh;
    double Vkh;
    double tau1h;
    double tau2h;

    double km;
    double lm;
    double Vlm;
    double Vkm;
    double tau1m;
    double tau2m;

    // specify parameters + initial conditions
    GenericChannel(double gbar_, double E_, double m_, double h_, double p_, double q_, double km_, double lm_, double Vlm_, double Vkm_, double tau1m_, double tau2m_, double kh_, double lh_, double Vlh_, double Vkh_, double tau1h_, double tau2h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        km = km_;
        lm = lm_;
        Vlm = Vlm_;
        Vkm = Vkm_;
        tau1m = tau1m_;
        tau2m = tau2m_;

        kh = kh_;
        lh = lh_;
        Vlh = Vlh_;
        Vkh = Vkh_;
        tau1h = tau1h_;
        tau2h = tau2h_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = 30; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }

        // cast into int
        p = (int) p_;
        q = (int) q_;

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

string GenericChannel::getClass(){return "GenericChannel";}


double GenericChannel::m_inf(double V, double Ca) {
    return 1.0/(1.0+exp((V-Vkm)*km));
}


double GenericChannel::h_inf(double V, double Ca) {
    return 1.0/(1.0+exp((V-Vkh)*kh));
}

double GenericChannel::tau_m(double V, double Ca) {
    return tau1m + tau2m/(1+exp((V-Vlm)*lm));
}

double GenericChannel::tau_h(double V, double Ca) {
    return tau1h + tau2h/(1+exp((V-Vlh)*lh));
}


#endif
