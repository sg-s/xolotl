// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Slow Ca conductance
// component source [Liu et al. 98](http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf)
//
#pragma once
class conductance;

//inherit conductance class spec
class CaS: public conductance {

public:

    // specify parameters + initial conditions
    CaS(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults 
        if (isnan (E)) { E = 30; }

        p = 3;
        q = 1;

        is_calcium = true;

        name = "CaS";
        
    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);


};


double CaS::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+33.0)/-8.1));}
double CaS::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+60.0)/6.2));}
double CaS::tau_m(double V, double Ca) {return 1.4 + 7.0/(exp((V+27.0)/10.0) + exp((V+70.0)/-13.0));}
double CaS::tau_h(double V, double Ca) {return 60.0 + 150.0/(exp((V+55.0)/9.0) + exp((V+65.0)/-16.0));}


