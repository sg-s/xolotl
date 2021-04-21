// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Fast low threshold calcium current in the leech
// component source [Hill et al., 2001](https://link.springer.com/article/10.1023/A:1011216131638)
// 

#ifndef CAF
#define CAF

//inherit conductance class spec
class CaF: public conductance {

public:

    //specify both gbar and erev and initial conditions
    CaF(double gbar_, double E_, double m_, double h_)
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
		
        name = "CaF";

        is_calcium = true;
    }
	
    double m_inf(double, double);
    double tau_m(double, double);
	
    double h_inf(double, double);
    double tau_h(double, double);	
	

};


double CaF::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V+46.7)*-0.6));}
double CaF::h_inf(double V, double Ca) {return 1.0/(1.0+exp((V+55.5)*0.35));}
double CaF::tau_m(double V, double Ca) {return 11.0 + 24.0/(cosh(-0.33*(V+46.7)));}
double CaF::tau_h(double V, double Ca) {return 60.0 + 310.0/(1.0+exp((V+55.0)*0.27));}

#endif
