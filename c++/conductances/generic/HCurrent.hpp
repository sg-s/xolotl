// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// generic hyper-polarization activated current 
// with customizable activation and timescale 
// component info: HCurrent with customizable Vhalf and tau
#ifndef HCURRENT
#define HCURRENT


//inherit conductance class spec
class HCurrent: public conductance {

public:

    double Vhalf = -75;
    double tau = 2e3;

    //specify both gbar and erev and initial conditions
    HCurrent(double gbar_, double E_, double m_, double Vhalf_, double tau_)
    {
        gbar = gbar_;
        Vhalf = Vhalf_;
        E = E_;
        tau = tau_;
        m = m_;

        // defaults 
        if (isnan(gbar)) { gbar = 0; }
        if (isnan(Vhalf)) { Vhalf = -75; }
        if (isnan(tau)) { tau = 2e3; }
        if (isnan (E)) { E = -20; }

        name = "HCurrent";


    }
    double m_inf(double, double);
    double tau_m(double, double);
    

};




double HCurrent::m_inf(double V, double Ca) {return 1.0/(1.0+exp((V-Vhalf)/5.5));}
double HCurrent::tau_m(double V, double Ca) {return tau;}

#endif
