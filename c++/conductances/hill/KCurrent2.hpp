// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Persistent potassium current in the model of leech heartbeat HCO neurons(Hill et al., 2001)

#ifndef KCURRENT2
#define KCURRENT2

//inherit conductance class spec
class KCurrent2: public conductance {

public:

    //specify both gbar and erev and initial conditions
    KCurrent2(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;	

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -70; }
		
    // exponents of m
        p = 2;
	name = "KCurrent2";
    }
	
    double m_inf(double, double);
    double tau_m(double, double);

};


double KCurrent2::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+20.0)*-0.083));}
double KCurrent2::tau_m(double V, double Ca) {return 57.0 + 43.0/(1.0+exp((V+35.0)*0.2));}

#endif
