
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
    
    
    
public:
    compartment(string Name);

    double A = 1;
    double dt = -1;  
    double sim_dt = -1;
    double InjectedCurrent = 0;
    double V = -60;
    double V_prev = -60;
    int n_steps = 1e3;

    double Ca = .01;
    double Ca_prev = .01;


    int * arr;

    // get arr pointer
    // const int * const getMat(size_t len2) const;

    //plhs[1] = mxCreateDoubleMatrix(1, n_steps, mxREAL);

    string Name = "AB";


    int n_channels = 0;
    vector <conductance*> channels;

    void integrate();
    void add(conductance*);
    void init();
    void set_n_steps(int n);


};



