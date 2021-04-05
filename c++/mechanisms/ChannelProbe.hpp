// _  _ ____ _    ____ ___ _
//  \/  |  | |    |  |  |  |
// _/\_ |__| |___ |__|  |  |___
//
// component info: Probe that reads out activation and inactivation variables of a channel

#ifndef CHANNELPROBE
#define CHANNELPROBE

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
        name = "ChannelProbe";
    }

    void connectConductance(conductance *);
    double getState(int);
    

};


double ChannelProbe::getState(int idx) {
    double v = 0;
    switch (idx) {
        case 0:
            v = channel->m;
            break;
        case 1:
            v =  channel->h;
            break;
        default:
            mexErrMsgTxt("Illegal getState index");
    }
    return v;
}


void ChannelProbe::connectConductance(conductance * channel_) {
    // connect to a channel
    channel = channel_;

    // make sure the compartment that we are in knows about us
    (channel->container)->addMechanism(this);

}






#endif
