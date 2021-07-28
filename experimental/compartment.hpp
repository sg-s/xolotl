
#pragma once

#include <vector>
using std::vector;

#include <string>
using std::string;

class xolotl;
class conductance;

class compartment
{
private:
    
    double sigma_g;
    double sigma_gE;

    double V_inf;
    
public:
    compartment(string name);

    double A = .0628;
    double Cm = 10;
    double dt = .1;  
    double sim_dt = .1;

    double V = -60;
    double V_prev = -60;
    int n_steps = 1e4;

    double Ca = .01;
    double Ca_prev = .01;
    double I_ext = 0;

    double* V_data =  new double[1];

    // get arr pointer
    // const int * const getMat(size_t len2) const;

    //plhs[1] = mxCreateDoubleMatrix(1, n_steps, mxREAL);

    string name = "AB";


    int n_channels = 0;
    vector <conductance*> allchannels;

    void integrate();
    void step();
    void add(conductance*);
    void init();
    void set_n_steps(int n);

    conductance* channels(int);


};



