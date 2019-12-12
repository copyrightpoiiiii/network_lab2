//
// Created by zhangqi on 2019/12/8.
//

#ifndef NETWORK_LAB2_TCPRDTRECEIVER_H
#define NETWORK_LAB2_TCPRDTRECEIVER_H

#include "RdtReceiver.h"
#include <queue>

class sort_Message : public Message {
public:
	int mesNum;

	bool operator< (const sort_Message &p) const {
		return mesNum < p.mesNum;
    }

    bool operator>(const sort_Message &p) const {
        return mesNum >= p.mesNum;
    }
};

class SelectRdtReceiver : public RdtReceiver {
private:
	sort_Message *waitAck;
	Packet lastAckPkt;
	int base;
	int receiveWindow;
	int rBase;
	int receiveSize;
public:
	SelectRdtReceiver ();

	virtual ~SelectRdtReceiver ();

public:

	bool inWindow (int pos);

	void receive (const Packet &packet);
};


#endif //NETWORK_LAB2_TCPRDTRECEIVER_H
