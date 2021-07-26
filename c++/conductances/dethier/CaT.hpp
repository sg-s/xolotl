// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// Deleyed rectifier
// Dethier, Drion, Franci, Sepulchre 2015
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4620135/

#pragma once


//inherit conductance class spec
class CaT: public conductance {

public:

    // specify parameters + initial conditions
    CaT(double gbar_, double E_, double m_, double h_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;
        h = h_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (h)) { h = 0; }
        if (isnan (E)) { E = 30; }


        p = 3;
        q = 1;

        is_calcium = true;
        name = "CaT";
    }


    double m_inf(double, double);
    double tau_m(double, double);
    double h_inf(double, double);
    double tau_h(double, double);
    


};




double CaT::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+57.1)/-7.2)));}
double CaT::h_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+82.1)/5.5)));}
double CaT::tau_m(double V, double Ca) {return 43.4- (42.6/(1.0+exp(((V)+68.1)/-20.5)));}
double CaT::tau_h(double V, double Ca) {return 840.0- (718.4/(1.0+exp(((V)+55.0)/-16.9)));}

