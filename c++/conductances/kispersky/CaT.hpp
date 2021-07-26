// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Transient Calcium Conductance
// http://www.jneurosci.org/content/32/32/10995
// Kispersky, Caplan, Marder 2012
#pragma once


//inherit conductance class spec
class CaT: public conductance {

public:


    //specify both gbar and erev and initial conditions
    CaT(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

         // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        
        if (isnan (E)) { E = 0; }

        p = 3;
        q = 1;

        is_calcium = true;

        name = "CaT";

    }


    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    

};



double CaT::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+27.1)/-7.2)));}
double CaT::tau_m(double V, double Ca) {return 21.7-(21.3/(1.0+exp(((V)+68.1)/-20.5)));}
double CaT::h_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+32.1)/5.5)));}
double CaT::tau_h(double V, double Ca) {return 105.0-((89.8)/(1.0+exp(((V)+55.0)/-16.9)));}


