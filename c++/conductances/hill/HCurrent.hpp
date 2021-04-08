// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Hyperpolarization activated inward  (H) current in the leech
// component source [Hill et al., 2001](https://link.springer.com/article/10.1023/A:1011216131638)
// 

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

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -21; }
		

    // exponents of m
        p = 2;

	name = "HCurrent";			
    }
	
    double m_inf(double, double);
    double tau_m(double, double);
	

};


double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0 + 2.0*exp((V+47.0)*0.18) + exp((V+47.0)*0.5));}
double HCurrent::tau_m(double V, double Ca) {return 700.0 + 1700.0/(1.0+exp((V+73.0)*-0.1));}

#endif
