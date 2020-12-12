// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// H Current from Sharp et al. 1996
// J Neurophys
// https://journals.physiology.org/doi/pdf/10.1152/jn.1996.76.2.867
#ifndef HCURRENT
#define HCURRENT
#include "conductance.hpp"

//inherit conductance class spec
class HCurrent: public conductance {

public:

    double Vhalf;

    HCurrent(double gbar_, double E_, double Vhalf_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
		Vhalf = Vhalf_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }    
        if (isnan (E)) { E = -10; }
		if (isnan (Vhalf)) { Vhalf = -50; }

        name = "HCurrent";

    }
	
    double m_inf(double, double);
    double tau_m(double, double);
	
    

};




double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V-Vhalf)/7));}
double HCurrent::tau_m(double V, double Ca) {return (1./(0.0005*(1+exp((V+110)/-13))));}


#endif
