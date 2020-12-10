// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Probe that reads out activation and inactivation variables of a channel

#ifndef CHANNELPROBE
#define CHANNELPROBE
#include "mechanism.hpp"
#include <limits>

//inherit controller class spec
class ChannelProbe: public mechanism {

protected:

public:

    // dummy member
    double dummy;

    // specify parameters + initial conditions for
    ChannelProbe(double dummy_) {
        dummy = dummy_;
    }


    void integrate(void);

    void checkSolvers(int);

    void init(void);

    void connect(conductance *);

    int getFullStateSize(void);
    int getFullState(double * cont_state, int idx);
    double getState(int);
    string getClass(void);

};


void ChannelProbe::init() {
    // do nothing
}

string ChannelProbe::getClass() {
    return "ChannelProbe";
}


double ChannelProbe::getState(int idx) {
    if (idx == 1) {return channel->m;}
    else if (idx == 2) {return channel->h;}
    else {return std::numeric_limits<double>::quiet_NaN();}

}


int ChannelProbe::getFullStateSize(){return 2; }


int ChannelProbe::getFullState(double *cont_state, int idx) {
    // read out m and h
    cont_state[idx] = channel->m;
    idx++;
    cont_state[idx] = channel->h;
    idx++;
    return idx;
}


void ChannelProbe::connect(conductance * channel_) {

    // connect to a channel
    channel = channel_;


    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);


}



void ChannelProbe::integrate(void) {
    // do nothing
}



void ChannelProbe::checkSolvers(int k) {
    // do nothing
}




#endif
