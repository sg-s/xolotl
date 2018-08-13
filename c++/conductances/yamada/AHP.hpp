// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// I_AHP current
// from 
// Yamada WM, Koch C, Adams P (1989) Multiple channels
// and calcium dynamics In: Methods in neuronal modeling.
// From synapses to networks (Koch C, Segev I, eds),
// pp 97-133. Cambridge, MA: MIT Press.
// 
// also found in 
// https://huguenardlab.stanford.edu/reprints/25_Cereb_Cortex-1993-McCormick-387-98.pdf

#ifndef AHP
#define AHP
#include "conductance.hpp"

//inherit conductance class spec
class AHP: public conductance {

public:

    // specify parameters + initial conditions
    AHP(double g_, double E_, double m_, double h_)
    {
        gbar = g_;
        E = E_;
        m = m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan (m)) { m = 0; }
        if (isnan (E)) { E = 0; }
    }

    void integrate(double V, double Ca, double dt, double delta_temp);

    double m_inf(double V, double Ca);
    double tau_m(double V, double Ca);
    string getClass(void);


};

string AHP::getClass(){return "AHP";}

void AHP::integrate(double V, double Ca, double dt, double delta_temp)
{

    m = m_inf(V, Ca) + (m - m_inf(V, Ca))*exp(-dt/tau_m(V, Ca));
    g = gbar*m;



}

double AHP::m_inf(double V, double Ca) {return 130.8*Ca*Ca/(130.8*Ca*Ca + .001);}

double AHP::tau_m(double V, double Ca) {return 1/(130.8*Ca*Ca + .001)}



#endif
