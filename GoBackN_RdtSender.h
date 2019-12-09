//
// Created by zhangqi on 2019/12/8.
//

#ifndef NETWORK_LAB2_GOBACKN_RDTSENDER_H
#define NETWORK_LAB2_GOBACKN_RDTSENDER_H

#include "RdtSender.h"


class GoBackN_RdtSender :public RdtSender
{
private:
    int expectSequenceNumberSend;	// 下一个发送序号
    bool waitingState;				// 是否处于等待Ack的状态
    Packet packetWaitingAck;		//已发送并等待Ack的数据包
    int sendWindow;//the window of send length

public:

    bool getWaitingState();
    bool send(const Message &message);						//发送应用层下来的Message，由NetworkServiceSimulator调用,如果发送方成功地将Message发送到网络层，返回true;如果因为发送方处于等待正确确认状态而拒绝发送Message，则返回false
    void receive(const Packet &ackPkt);						//接受确认Ack，将被NetworkServiceSimulator调用
    void timeoutHandler(int seqNum);					//Timeout handler，将被NetworkServiceSimulator调用

public:
    GoBackN_RdtSender();
    virtual ~GoBackN_RdtSender();
};


#endif //NETWORK_LAB2_GOBACKN_RDTSENDER_H