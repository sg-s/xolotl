// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Fast sodium current in the model of leech heartbeat HCO neurons (Hill et al., 2001)

#ifndef NAV
#define NAV

//inherit conductance class spec
class NaV: public conductance {

public:

    //specify both gbar and erev and initial conditions
    NaV(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;		

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = 45; }
		
    // exponents of m and h
        p = 3;
        q = 1;

	name = "NaV";
    }
	
    double m_inf(double, double);
    double tau_m(double, double);	
    double h_inf(double, double);
    double tau_h(double, double);	
	
};


double NaV::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+29.0)*-0.15));}
double NaV::tau_m(double V, double Ca) {return 0.1;}
double NaV::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+30.0)*0.5));}
double NaV::tau_h(double V, double Ca) {return (4.0 + 6.0/(1+exp((V+28.0)*0.5)) + 10.0/(cosh(0.3*(V+27.0))));}

#endif
