// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Probe that reads out activation and inactivation variables of a channel

#ifndef CHANNELPROBE
#define CHANNELPROBE
#include "mechanism.hpp"

//inherit controller class spec
class ChannelProbe: public mechanism {

protected:

public:

    // dummy member
    double dummy;

    // specify parameters + initial conditions for
    ChannelProbe(double dummy_) {
        dummy = dummy_;
        fullStateSize = 2;
    }

    void connectConductance(conductance *);

    int getFullState(double * cont_state, int idx);
    string getClass(void);

};




string ChannelProbe::getClass() {
    return "ChannelProbe";
}



int ChannelProbe::getFullState(double *cont_state, int idx) {
    // read out m and h
    cont_state[idx] = channel->m;
    idx++;
    cont_state[idx] = channel->h;
    idx++;
    return idx;
}


void ChannelProbe::connectConductance(conductance * channel_) {

    // connect to a channel
    channel = channel_;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

}






#endif
