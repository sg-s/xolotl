// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: inward rectifying (K+) current
// component source [Amarillo et al. 98](https://journals.physiology.org/doi/pdf/10.1152/jn.00867.2017)
//
#ifndef KIR
#define KIR


//inherit conductance class spec
class Kir: public conductance {

public:

    //specify both gbar and erev and initial conditions
    Kir(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults 
        if (isnan (E)) { E = -100; }

        unitary_conductance = 20e-6; // uS

        name = "Kir";

        p = 1;
        instantaneous_m = 1;

    }

    double m_inf(double, double);
    

};


double Kir::m_inf(double V, double Ca) {
    return 1/(1+exp((V+97.9)/9.7));
}


#endif
