//
// Created by zhangqi on 2019/12/8.
//

#ifndef NETWORK_LAB2_GOBACKN_RDTRECEIVER_H
#define NETWORK_LAB2_GOBACKN_RDTRECEIVER_H

#include "../header/RdtReceiver.h"

class GoBackN_RdtReceiver :public RdtReceiver{
private:
    int expectSequenceNumberRcvd;
    Packet lastAckPkt;
public:
    GoBackN_RdtReceiver();
    virtual ~GoBackN_RdtReceiver();

public:
    void receive(const Packet &packet);
};


#endif //NETWORK_LAB2_GOBACKN_RDTRECEIVER_H
