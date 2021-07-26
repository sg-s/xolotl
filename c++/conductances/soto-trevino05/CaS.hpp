// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow Calcium conductance
// http://jn.physiology.org/content/94/1/590.short
#pragma once


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

        p = 3;


        // defaults
        if (isnan(gbar)) { gbar = 0; }        
        if (isnan (E)) { E = 50; }

        is_calcium = true;

        name = "CaS";

    }


    double m_inf(double, double);
    double tau_m(double, double);
    

};



double CaS::m_inf(double V, double Ca) {return 1.0/(1.0+exp(-(V+22.0)/8.5));}
double CaS::tau_m(double V, double Ca) {return 16 - 13.1/(1 + exp(-(V+25.1)/-26.4));}


