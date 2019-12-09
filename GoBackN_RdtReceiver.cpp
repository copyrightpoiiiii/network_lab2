//
// Created by zhangqi on 2019/12/8.
//

#include "GoBackN_RdtReceiver.h"
#include "Global.h"

GoBackN_RdtReceiver::GoBackN_RdtReceiver() {
    expectSequenceNumberRcvd=0;
    lastAckPkt.acknum= -1;
    lastAckPkt.checksum=0;
    lastAckPkt.seqnum =-1;
    for(int i=0;i<Configuration::PAYLOAD_SIZE;i++)
            lastAckPkt.payload[i]='.';
    lastAckPkt.checksum=pUtils->calculateCheckSum(lastAckPkt);
}

GoBackN_RdtReceiver::~GoBackN_RdtReceiver(){

}

void GoBackN_RdtReceiver::receive(const Packet &packet) {
    int checkSum=pUtils->calculateCheckSum(packet);

    if(checkSum == packet.checksum && this->expectSequenceNumberRcvd == packet.seqnum) {
        pUtils->printPacket("接收方正确收到发送方的报文", packet);
        Message msg;
        memcpy(msg.data, packet.payload, sizeof(packet.payload));
        pns->delivertoAppLayer(RECEIVER, msg);

        lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
        lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
        pUtils->printPacket("接收方发送确认报文", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);	//调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方

        this->expectSequenceNumberRcvd+=1 ; //every time num will increase 1
    }
    else {
        if (checkSum != packet.checksum) {
            pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
        }
        else {
            pUtils->printPacket("接收方没有正确收到发送方的报文,报文序号不对", packet);
        }
        pUtils->printPacket("接收方重新发送上次的确认报文", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);
    }
}