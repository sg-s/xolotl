// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// the A current, a potassium current
// Brookings, Goeritz, Marder 2014
// https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4274917/?report=printable#app1
#ifndef ACURRENT
#define ACURRENT


//inherit conductance class spec
class ACurrent: public conductance {

public:

    // specify parameters + initial conditions
    ACurrent(double gbar_, double E_, double m_)
    {
        gbar = gbar_;
        E = E_;
        m = m_;

        // defaults
        if (isnan(gbar)) { gbar = 0; }
        
        if (isnan (E)) { E = -80; }

        p = 3;

        name = "ACurrent";

    }


    double m_inf(double V, double Ca);
    double h_inf(double V, double Ca);
    double tau_m(double V, double Ca);
    double tau_h(double V, double Ca);

};





double ACurrent::m_inf(double V, double Ca) {return (1.0/(1.0+exp(((V)+12.3)/-11.8))); }
double ACurrent::tau_m(double V, double Ca) {return 7.2- (6.4/(1.0+exp(((V)+28.3)/-19.2))); }

double ACurrent::h_inf(double V, double Ca) {return   0;}
double ACurrent::tau_h(double V, double Ca) {return   0;}

#endif
