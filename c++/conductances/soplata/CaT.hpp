// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: T-type calcium current
// component source: [Soplata et al. 2017](https://www.ncbi.nlm.nih.gov/pubmed/29227992)
//
// direct link to supplementary info: https://journals.plos.org/ploscompbiol/article/file?type=supplementary&id=info:doi/10.1371/journal.pcbi.1005879.s001
// direct link to GitHub repository: https://github.com/asoplata/propofol-coupling-2017-full
//
// Soplata AE, McCarthy MM, Sherfey J, Lee S, Purdon PL, Brown EN, et al.
// (2017) Thalamocortical control of propofol phase-amplitude coupling. PLoS
// Comput Biol 13(12): e1005879. https://doi.org/10.1371/journal.pcbi.1005879
//
// dynamics were inherited from Ching et al. 2010,
// but diverge from the original publication
//
// Ching, S., Cimenser, A., Purdon, P. L., Brown, E. N., & Kopell, N. J.
// (2010). Thalamocortical model for a propofol-induced alpha-rhythm
// associated with loss of consciousness. Proceedings of the National
// Academy of Sciences, 107(52), 22665–22670.
// http://doi.org/10.1073/pnas.1017069108

#ifndef CAT
#define CAT
#include "conductance.hpp"

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
        if (isnan (E)) { E = 30; }

        p = 2;
        q = 1;

        // allow this channel to be approximated
        approx_m = 1;
        approx_h = 1;

        is_calcium = true;

    }

    void integrate(double, double);
    double m_inf(double, double);
    double h_inf(double, double);
    double tau_m(double, double);
    double tau_h(double, double);
    string getClass(void);


};

string CaT::getClass(){
    return "CaT";
}


double CaT::m_inf(double V, double Ca) {return 1 / (1 + exp((-(V + 2) + 57)/6.2));}
double CaT::h_inf(double V, double Ca) {return 1 / (1 + exp(((V + 2) + 81)/4));}
double CaT::tau_m(double V, double Ca) {return 0;}
double CaT::tau_h(double V, double Ca) {return (30.8 + (211.4 * exp(((V + 2) + 113.2)/5))/(1 + exp(((V + 2) + 84)/3.2))) / 3.73;}

// define a custom integrate method
// to deal with the fact that the m-kinetics are instantaneous

void CaT::integrate(double V, double Ca) {


    E = container->E_Ca;
    
    V_idx = (int) round((V*10)+999);
    if (V_idx < 0) {V_idx = 0;};
    if (V_idx > 2000) {V_idx = 2000;};

    // there is no scenario where approx_m and approx_h are different
    // so we need only one switch
    switch (approx_m) {
        case 0:
            minf = m_inf(V,Ca);
            m = minf;
            hinf = h_inf(V,Ca);
            h = hinf + (h - hinf)*exp(-dt/tau_h(V,Ca));
            break;

        default:
            m = m_inf_cache[V_idx];
            h = h_inf_cache[V_idx] + (h - h_inf_cache[V_idx])*fast_exp(-(dt/tau_h_cache[V_idx]));
    } // switch approx_m

    g = gbar*fast_pow(m,p)*fast_pow(h,q);;

    container->i_Ca += getCurrent(V);


}



#endif
