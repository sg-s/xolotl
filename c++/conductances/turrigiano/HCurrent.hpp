// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// H current. again, for mysterious reasons, the compiler
// won't let me call this class "H"
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef HCURRENT
#define HCURRENT


//inherit conductance class spec
class HCurrent: public conductance {

public:

    //specify both gbar and erev and initial conditions
    HCurrent(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        p = 1;

        name = "HCurrent";

        // defaults
        if (isnan(gbar)) { gbar = 0; }        
        if (isnan (E)) { E = -20; }

    }

    double m_inf(double, double);
    double tau_m(double, double);
    
};


double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+78.3)/6.5));}
double HCurrent::tau_m(double V, double Ca) {return (272.0 + 1499.0/(1.0+exp((V+42.2)/-8.73)));}


#endif
