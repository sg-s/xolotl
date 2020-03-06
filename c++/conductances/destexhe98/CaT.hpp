// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// low-threshold Ca++ current
// from
// Dendritic Low-Threshold Calcium Currents in Thalamic Relay Cells
// Alain Destexhe, Mike Neubig, Daniel Ulrich and John Huguenard
// https://www.jneurosci.org/content/18/10/3574.short


#ifndef CAT
#define CAT
#include "conductance.hpp"

//inherit conductance class spec
class CaT: public conductance {


private:

    // K = 4*F*F/(R*T)
    double K;

    // K_exp = exp(-ZFV/RT)
    double K_exp;

public:


    // maximum permeability
    // in units of cm/s
    double pbar; 

    // specify parameters + initial conditions
    CaT(double gbar_, double E_, double m_, double h_, double pbar_)
    {

        

        pbar = pbar_;

        E = E_;
        m = m_;
        h = h_;

        // gbar and E are unused in this model
        gbar = gbar_;
        E = E_;

        // force these values, because they will not be used
        gbar = 0;
        E = 0;

        p = 2;
        q = 1;

        if (isnan(pbar)) {pbar = 0;}

        // allow this channel to be approximated
        approx_m = 0;
        approx_h = 0;

        is_calcium = true;

    }


    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);

    void integrate(double, double);

    string getClass(void);

    void connect(compartment *);


    double G_by_V(double, double);

    double getCurrent(double);

};


void CaT::connect(compartment * pcomp_) {
    conductance::connect(pcomp_);

    // compute a constant
    K = (4.6469e4)/(temperature+273.16);
    // units of C/J


}

double CaT::getCurrent(double V) {
    return V*G_by_V(V, container->Ca_prev);
}


double CaT::G_by_V(double V, double Ca) {
    K_exp = exp(-V/(container->RT_by_nF));
    return K*(Ca - container->Ca_out * K_exp)/(1 - K_exp);

}

void CaT::integrate(double V, double Ca) {


    // integrate the gating variables
    minf = m_inf(V,Ca);
    m = minf + (m - minf)*exp(-dt/tau_m(V,Ca));


    hinf = h_inf(V,Ca);
    h = hinf + (h - hinf)*exp(-dt/tau_h(V,Ca));

    // Equation 1 and 2 can be re-written to be in the form
    // I = gV - gE
    // where E = 0. Then, g is given by

    g = pbar*m*m*h*G_by_V(V, Ca);


    container->i_Ca += g*V;
}


string CaT::getClass(){
    return "CaT";
}


double CaT::m_inf(double V, double Ca) {
    return 1.0 / ( 1.0 + exp( (V  + 56.0) / -6.2 ) );
}

double CaT::h_inf(double V, double Ca)  {
    return 1.0 / (1.0 + exp( ( V  + 80.0) / 4.0 ) ); 
}

double CaT::tau_m(double V, double Ca)  {
    return ( 0.204 + .333 / ( exp( (V + 131.0 ) / -16.7) + exp( (V + 15.8 ) / 18.2) ) );

}

double CaT::tau_h(double V, double Ca)  {
    if (V < -81)  {
        return 0.333* exp((V+466)/(66.6));
    } else {
        return 9.32 + 0.333 * exp((V+21)/(-10.5));

    }
}



#endif
