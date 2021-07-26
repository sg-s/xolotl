// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Fast transient potassium current in the leech
// component source [Hill et al., 2001](https://link.springer.com/article/10.1023/A:1011216131638)
// 

#pragma once

//inherit conductance class spec
class KACurrent: public conductance {

public:

    //specify both gbar and erev and initial conditions
    KACurrent(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
		h = h_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -70; }
		
    // exponents of m
        p = 2;
        q = 1;

	name = "KACurrent";
    }
	
    double m_inf(double, double);
    double tau_m(double, double);
	
    double h_inf(double, double);
    double tau_h(double, double);	
	
};


double KACurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+44.0)*-0.13));}
double KACurrent::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+63.0)*0.16));}
double KACurrent::tau_m(double V, double Ca) {return 5.0 + 11.0/(1.0+exp((V+30.0)*0.2));}
double KACurrent::tau_h(double V, double Ca) {return 26.0 + 8.5/(1.0+exp((V+55.0)*-0.3));}

