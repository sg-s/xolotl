// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Slow low threshold calcium current in the model of leech heartbeat HCO neurons (Hill et al., 2001)

#ifndef CAS
#define CAS


//inherit conductance class spec
class CaS: public conductance {

public:

    //specify both gbar and erev and initial conditions
    CaS(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;		

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = 135; }
		
    // exponents of m and h
        p = 2;
        q = 1;

name = "CaS";		
    }
	
    double m_inf(double, double);
    double tau_m(double, double);
	
    double h_inf(double, double);
    double tau_h(double, double);	

};


double CaS::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+47.2)*-0.42));}
double CaS::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+55.0)*0.36));}
double CaS::tau_m(double V, double Ca) {return (5.0 + 134.0/(1+exp((V+48.7)*-0.4)));}
double CaS::tau_h(double V, double Ca) {return (200.0 + 5250.0/(1+exp((V+43.0)*-0.25)));}

#endif
