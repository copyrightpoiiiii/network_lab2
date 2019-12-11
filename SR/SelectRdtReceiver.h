//
// Created by zhangqi on 2019/12/8.
//

#ifndef NETWORK_LAB2_SELECTRDTRECEIVER_H
#define NETWORK_LAB2_SELECTRDTRECEIVER_H

#include "../header/RdtReceiver.h"
#include <queue>

struct sort_Packet : public Message {
	int mesNum;

	bool operator< (const sort_Packet &p) const {
		return p.mesNum < this->mesNum;
	}
};

class SelectRdtReceiver : public RdtReceiver {
private:
	std::priority_queue<sort_Packet *> waitAck;
	Packet lastAckPkt;
	int base;
	int receiveWindow;
	int rBase;
	int receiveSize;
public:
	SelectRdtReceiver ();

	virtual ~SelectRdtReceiver ();

public:
	void receive (const Packet &packet);
};


#endif //NETWORK_LAB2_SELECTRDTRECEIVER_H
