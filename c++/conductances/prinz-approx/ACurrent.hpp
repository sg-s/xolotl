// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// the A current, a potassium current
// for some reason I don't understand, I get compiler
// errors when I name this "A" or "Ka"
// so we'll have to live with this awkward name
// http://www.jneurosci.org/content/jneuro/18/7/2309.full.pdf
#ifndef ACURRENT
#define ACURRENT
#include "conductance.hpp"

//inherit conductance class spec
class ACurrent: public conductance {

public:

    // specify parameters + initial conditions
    ACurrent(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;
        h = h_;


        // defaults 
 if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (h)) { h = 1; }
        if (isnan (E)) { E = -80; }


        // cache values for m_inf and h_inf
        for (double V = -99; V < 101; V++) {
            m_inf_cache[(int) round(V+99)] = m_inf(V);
            h_inf_cache[(int) round(V+99)] = h_inf(V);
            tau_m_cache[(int) round(V+99)] = tau_m(V);
            tau_h_cache[(int) round(V+99)] = tau_h(V);
        }

    }

    double m_inf_cache[200];
    double h_inf_cache[200];
    double tau_m_cache[200];
    double tau_h_cache[200];

    double taum;
    double tauh;
    double minf;
    double hinf;

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V);
    double h_inf(double V);
    double tau_m(double V);
    double tau_h(double V);
    string getClass(void);


};

string ACurrent::getClass(){return "ACurrent";}

void ACurrent::integrate(double V, double Ca, double dt, double delta_temp)
{

    // clamp the voltage inside of cached range
    if (V > 101.0) {V = 101.0;}
    else if (V < -99.0) {V = -99.0;}

    minf = m_inf_cache[(int) round(V+99)];
    hinf = h_inf_cache[(int) round(V+99)];
    taum = tau_m_cache[(int) round(V+99)];
    tauh = tau_h_cache[(int) round(V+99)];

    m = minf + (m - minf)*exp(-(dt/taum));
    h = hinf + (h - hinf)*exp(-(dt/tauh));
    g = gbar*m*m*m*h;
}

double ACurrent::m_inf(double V) {return 1.0/(1.0+exp((V+27.2)/-8.7)); }
double ACurrent::h_inf(double V) {return 1.0/(1.0+exp((V+56.9)/4.9)); }
double ACurrent::tau_m(double V) {return 23.2 - 20.8/(1.0+exp((V+32.9)/-15.2));}
double ACurrent::tau_h(double V) {return 77.2 - 58.4/(1.0+exp((V+38.9)/-26.5));}

#endif
