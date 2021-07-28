#pragma once

#include <vector>
using std::vector;

class thing;

class mycontainer
{
private:
    
public:
    mycontainer() {}
    void addThing(thing * thing_);

    double value = 1;

    void init();

    vector <thing*> things;
    int n_things = 0;


};




