//
// Created by zhangqi on 2019/12/8.
//

#ifndef NETWORK_LAB2_TCPRDTRECEIVER_H
#define NETWORK_LAB2_TCPRDTRECEIVER_H

#include "RdtReceiver.h"
#include <queue>

class sort_Message_tcp : public Message {
public:
    int mesNum;

    bool operator<(const sort_Message_tcp &p) const {
        return mesNum < p.mesNum;
    }

    bool operator>(const sort_Message_tcp &p) const {
        return mesNum >= p.mesNum;
    }
};

class TcpRdtReceiver : public RdtReceiver {
private:
    std::priority_queue<sort_Message_tcp, std::vector<sort_Message_tcp>, std::greater<sort_Message_tcp> > waitAck;
    Packet lastAckPkt;
    int base;
    int receiveWindow;
    int rBase;
    int receiveSize;
public:
    TcpRdtReceiver();

    virtual ~TcpRdtReceiver();

public:
	void receive (const Packet &packet);
};


#endif //NETWORK_LAB2_TCPRDTRECEIVER_H
