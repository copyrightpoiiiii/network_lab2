//
// Created by zhangqi on 2019/12/8.
//

#ifndef NETWORK_LAB2_SELECTRDTSENDER_H
#define NETWORK_LAB2_SELECTRDTSENDER_H

#include "RdtSender.h"
#include <list>


struct sort_Packet : public Packet {
    bool acked = false;
};

class SelectRdtSender : public RdtSender {
private:
	int expectSequenceNumberSend;    // 下一个发送序号
	bool waitingState;                // 是否处于等待Ack的状态
	std::list<sort_Packet *> packetWaitingAck;//已发送并等待Ack的数据包
	int sendWindow;//the window of send length
	int sendSize;//the number of packets needs to seed
	int base;//start of the window
	int rBase;//end of the window
public:
	bool getWaitingState ();

	bool send (const Message &message);

	bool inWindow (int pos);

	void receive (const Packet &ackPkt);

	void timeoutHandler (int seqNum);

public:
	SelectRdtSender ();

	virtual ~SelectRdtSender ();
};


#endif //NETWORK_LAB2_TCPRDTSENDER_H
