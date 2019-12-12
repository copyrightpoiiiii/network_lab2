//
// Created by zhangqi on 2019/12/8.
//

#include "../header/SelectRdtReceiver.h"
#include "../header/Global.h"

SelectRdtReceiver::SelectRdtReceiver() {
    lastAckPkt.acknum = -1;
    lastAckPkt.checksum = 0;
    lastAckPkt.seqnum = -1;
    for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++)
        lastAckPkt.payload[i] = '.';
    lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
    base = 0;
    receiveWindow = Configuration::WINDOW_N;
    rBase = receiveWindow / 2;
    receiveSize = Configuration::MAX_SIZE;
    waitAck = (sort_Message **) malloc(receiveWindow * sizeof(sort_Message **));
    for (int i = 0; i < receiveWindow; i++) {
        waitAck[i] = nullptr;
    }
}

SelectRdtReceiver::~SelectRdtReceiver () {
    for (int i = 0; i < receiveWindow; i++) {
        if (waitAck[i])
            free(waitAck[i]);
    }
}

bool SelectRdtReceiver::inWindow (int pos) {
	int l = (pos - base + receiveWindow) % receiveWindow;
	int r = (rBase - base + receiveWindow) % receiveWindow;
	return l < r;
}

void SelectRdtReceiver::receive (const struct Packet &packet) {
	int checkSum = pUtils->calculateCheckSum (packet);
	if (checkSum == packet.checksum && inWindow (packet.seqnum)) {
        pUtils->printPacket("接收方正确收到发送方的报文", packet);

        sort_Message *msg = new sort_Message;
        memcpy(msg->data, packet.payload, sizeof(packet.payload));
        msg->mesNum = packet.seqnum;
        waitAck[msg->mesNum] = msg;
        while (waitAck[base] != nullptr) {
            pns->delivertoAppLayer(RECEIVER, *(waitAck[base]));
            free(waitAck[base]);
            waitAck[base] = nullptr;
            base++;
            base %= receiveWindow;
        }
        rBase = (base + receiveWindow / 2) % receiveWindow;
        //pns->delivertoAppLayer(RECEIVER, msg);

        lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
        lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
        pUtils->printPacket("接收方发送确认报文", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);    //调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方

        //this->expectSequenceNumberRcvd+=1 ; //every time num will increase 1
    } else if (checkSum == packet.checksum) {
        lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
        lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
        pUtils->printPacket("接收方发送确认报文", lastAckPkt);
        pns->sendToNetworkLayer(SENDER, lastAckPkt);    //调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
    } else {
        std::cout << base << " " << rBase << std::endl;
    }
    /* else {
         if (checkSum != packet.checksum) {
             pUtils->printPacket("接收方没有正确收到发送方的报文,数据校验错误", packet);
         } else {
             pUtils->printPacket("接收方没有正确收到发送方的报文,报文序号不对", packet);
         }

         lastAckPkt.acknum = base; //确认序号等于收到的报文序号
         lastAckPkt.checksum = pUtils->calculateCheckSum(lastAckPkt);
         pUtils->printPacket("接收方重新发送上次的确认报文", lastAckPkt);
         pns->sendToNetworkLayer(SENDER, lastAckPkt);
     }*/
}

