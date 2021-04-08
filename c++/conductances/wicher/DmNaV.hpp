// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// DmNaV (para) in Drosophila
// http://www.sciencedirect.com/science/article/pii/S0301008200000666
#ifndef DMNAV
#define DMNAV


//inherit conductance class spec
class DmNaV: public conductance {

public:

    // specify parameters + initial conditions
    DmNaV(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        p = 3;
        q = 1;

        name = "DmNaV";

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = 50; }

    }

    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    
};


double DmNaV::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+25.5)/-5.29));}
double DmNaV::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+48.9)/5.18));}
double DmNaV::tau_m(double V, double Ca) {return 1.32 - 1.26/(1+exp((V+120.0)/-25.0));}
double DmNaV::tau_h(double V, double Ca) {return (0.67/(1.0+exp((V+62.9)/-10.0)))*(1.5+1.0/(1.0+exp((V+34.9)/3.6)));}

#endif
