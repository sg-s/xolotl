#pragma once
#include <vector>
using std::vector;

class compartment;


class xolotl
{
private:
    
public:
    xolotl() {};
    

    double dt = 1;
    double sim_dt = 1;

    


    vector <compartment*> allcompartments;
    int n_compartments = 0;


    void init();
    void integrate();
    void add(compartment* comp_);
    compartment* compartments(int);

};



