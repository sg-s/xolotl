// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Inactivating Calcium conductance
// from Turrigiano ... Marder 1995
// http://www.jneurosci.org/content/jneuro/15/5/3640.full.pdf

#ifndef CA2
#define CA2


//inherit conductance class spec
class Ca2: public conductance {

public:

    // specify parameters + initial conditions
    Ca2(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        p = 1;


        name = "Ca2";
        is_calcium = true;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (E)) { E = 120; }

    }


    double m_inf(double, double);
    double tau_m(double, double);
    
};


double Ca2::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+21.6)/-8.5));}
double Ca2::tau_m(double V, double Ca) {return 16 - 13.1/(exp((V+25.1)/-26.4));}

#endif
