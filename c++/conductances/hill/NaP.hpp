// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Persistent sodium current in the leech
// component source [Hill et al., 2001](https://link.springer.com/article/10.1023/A:1011216131638)
// 

#ifndef NAP
#define NAP

//inherit conductance class spec
class NaP: public conductance {

public:

    //specify both gbar and erev and initial conditions
    NaP(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = 45; }
		
	name = "NaP";
    }
	
    double m_inf(double, double);
    double tau_m(double, double);	
	
};


double NaP::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+39.0)*-0.12));}
double NaP::tau_m(double V, double Ca) {return 10.0 + 200.0/(1.0+exp(0.4*(V+57.0)));}


#endif
