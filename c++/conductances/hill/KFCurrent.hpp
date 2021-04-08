// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// FMRFamide (Phe-Met-Arg-Phe-NH2) activated potassium current in the model of leech heartbeat HCO neurons (Hill et al., 2001)
// in a canonical model set to 0

#ifndef KFCURRENT
#define KFCURRENT

//inherit conductance class spec
class KFCurrent: public conductance {

public:

    //specify both gbar and erev and initial conditions
    KFCurrent(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -70; }
	
	name = "KFCurrent";	
    }
	
    double m_inf(double, double);
    double tau_m(double, double);
	
};


double KFCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+22.0)*-0.1));}
double KFCurrent::tau_m(double V, double Ca) {return 1500 + 800.0/(1.0+exp((V+22.0)*-0.1));}

#endif
