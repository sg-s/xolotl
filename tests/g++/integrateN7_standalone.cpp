// integrateN7_standalone.cpp
// Simple single-comparment neuron with 7 conductances
// based on Tim O'Leary's parameters and simulations 
// this is meant to be compiled and run externally 

#include <cmath>
#include <vector>
#include <typeinfo>
#include "../../compartment.hpp"

#include "../../conductances/liu/NaV.hpp"
#include "../../conductances/liu/CaT.hpp"
#include "../../conductances/liu/CaS.hpp"
#include "../../conductances/liu/ACurrent.hpp"
#include "../../conductances/liu/KCa.hpp"
#include "../../conductances/liu/Kd.hpp"
#include "../../conductances/liu/HCurrent.hpp"
#include "../../conductances/Leak.hpp"


using namespace std;

int main()
{
    double *output_V;
    double *output_Ca;
    int nits = 10;
    double dt, tstop;
    dt  = 50e-3;
    tstop = 50000;
    // default conductances
    double gbar_na = 1830.0, gbar_cat = 23.0, gbar_cas = 27.0, gbar_a = 246.0, gbar_kca = 980.0, gbar_kd = 610.0, gbar_h = 10.1, gbar_leak = .99;
    // default reversal potentials
    double e_leak = -50.0, e_na = 30.0, e_k = -80.0, e_h = -20.0;
    // default initial conditions
    double V0 = -60.0, Ca0 = 0.05, mNa = 0, mCaT = 0, mCaS = 0, mA = 0, mKCa = 0 , mKd = 0, mH = 0, hNa = 0, hCaT = 0, hCaS = 0, hA = 0;
    // default parameters
    double Ca_in = 0.05, Ca_out = 3000.0, tau_Ca = 200.0, Cm = 10.0, A = .0628,  vol = 1, phi = 1443;

    
    // make conductances
    Leak gleak(gbar_leak,e_leak);
    NaV gna(gbar_na,e_na, mNa, hNa);
    CaT gcat(gbar_cat,120.0, mCaT ,hCaT); // this needs to be replaced..
    CaS gcas(gbar_cas,120.0, mCaS, hCaS); // with the actual Ca rev potential
    ACurrent gka(gbar_a,e_k, mA ,hA);
    KCa gkca(gbar_kca,e_k, mKCa);
    Kd gkd(gbar_kd,e_k, mKd);
    HCurrent gh(gbar_h,e_h, mH);
    
    // make compartment and add all the conductances
    compartment cell(V0, Ca0, Cm, A, vol, phi, Ca_out, Ca_in, tau_Ca);
    cell.addConductance(&gna);
    cell.addConductance(&gcat);
    cell.addConductance(&gcas);
    cell.addConductance(&gka);
    cell.addConductance(&gkca);
    cell.addConductance(&gkd);
    cell.addConductance(&gh);
    cell.addConductance(&gleak);
    
    nits = (int) floor(tstop/dt);

    for(int i = 0; i < nits; i++)
    {
        (void) cell.integrate(dt);

        //output_V[i] = cell.V;
        //output_Ca[i] = cell.Ca;

    }
    return 0;
}
