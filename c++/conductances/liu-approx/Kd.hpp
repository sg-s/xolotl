// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// inward rectifying potassium conductance
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef KD
#define KD
#include "conductance.hpp"

//inherit conductance class spec
class Kd: public conductance {

public:

    //specify both gbar and erev and initial conditions
    Kd(double g_, double E_, double m_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = -80; }

        // cache values for m_inf and h_inf
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V);
            tau_m_cache[(int) round(V+99)] = tau_m(V);
        }

    }


    double m_inf_cache[200];
    double tau_m_cache[200];

    double taum;
    double minf;

    void integrate(double V, double Ca, double dt, double delta_temp);
    void connect(compartment *pcomp_);
    double m_inf(double V);
    double tau_m(double V);
    string getClass(void);


};

string Kd::getClass(){return "Kd";}

void Kd::connect(compartment *pcomp_) { container = pcomp_; }

void Kd::integrate(double V, double Ca, double dt, double delta_temp)
{

    // clamp the voltage inside of cached range
    if (V > 101.0)
    {
        V = 101.0;
    }

    if (V < -99.0)
    {
        V = -99.0;
    }
    
    minf = m_inf_cache[(int) round(V+99)];
    taum = tau_m_cache[(int) round(V+99)];

    m = minf + (m - minf)*exp(-(dt/taum));

    g = gbar*m*m*m*m;
}



double Kd::m_inf(double V) {return 1.0/(1.0+exp((V+12.3)/-11.8));}
double Kd::tau_m(double V) {return 7.2 - 6.4/(1.0+exp((V+28.3)/-19.2));}


#endif
