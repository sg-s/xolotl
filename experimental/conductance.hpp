
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


    double V;
    double Ca;

    double minf;
    double hinf;
    double taum;
    double tauh;
    
public:
    conductance(){};


    string name = "unset";

    double dt = -1;  
    double E = -50;
    double g = 0;
    double gbar = 0;

    compartment* container = nullptr;


    double m = 1;
    double h = 1;
    
    virtual void init();
    virtual void step();
    
    virtual double m_inf(double);
    virtual double h_inf(double);
    virtual double tau_m(double);
    virtual double tau_h(double); 



};


