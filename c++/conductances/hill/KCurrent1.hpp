// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Delayed rectifier like potassium current in the model of leech heartbeat HCO neurons (Hill et al., 2001)

#ifndef KCURRENT1
#define KCURRENT1

//inherit conductance class spec
class KCurrent1: public conductance {

public:

    //specify both gbar and erev and initial conditions
    KCurrent1(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;		

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -70; }
		
    // exponents of m and h
        p = 2;
        q = 1;

	name = "KCurrent1";
    }
	
    double m_inf(double, double);
    double tau_m(double, double);
	
    double h_inf(double, double);
    double tau_h(double, double);	
	
};


double KCurrent1::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+21.0)*-0.143));}
double KCurrent1::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+28.0)*0.111));}
double KCurrent1::tau_m(double V, double Ca) {return 1.0 + 11.0/(1+exp((V+16.0)*0.15));}
double KCurrent1::tau_h(double V, double Ca) {return 500.0 + 200.0/(1+exp((V+13.0)*-0.143));}

#endif
