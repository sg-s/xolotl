
#pragma once


#include <string>
#include <cmath>
using std::string;

class compartment;


class conductance
{


protected:
    int p;
    int q;
    string name = "unset";

    double V;
    double Ca;

    double minf;
    double hinf;
    double taum;
    double tauh;
    
public:
    conductance(){};


    double dt = -1;  
    double E = -50;
    double g = 1;
    double gbar = 0;

    compartment* container = nullptr;


    double m = 1;
    double h = 1;
    

    virtual void integrate();
    virtual double m_inf(double, double) = 0;
    virtual double h_inf(double, double) = 0;
    virtual double tau_m(double, double) = 0;
    virtual double tau_h(double, double) = 0; 



};


