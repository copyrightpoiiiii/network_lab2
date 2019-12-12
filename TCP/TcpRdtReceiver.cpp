//
// Created by zhangqi on 2019/12/8.
//

#include "../header/TcpRdtReceiver.h"
#include "../header/Global.h"

SelectRdtReceiver::SelectRdtReceiver () {
	lastAckPkt.acknum = -1;
	lastAckPkt.checksum = 0;
	lastAckPkt.seqnum = -1;
	for (int i = 0; i < Configuration::PAYLOAD_SIZE; i++)
		lastAckPkt.payload[i] = '.';
	lastAckPkt.checksum = pUtils->calculateCheckSum (lastAckPkt);
	base = 0;
	receiveWindow = Configuration::WINDOW_N;
	rBase = receiveWindow;
	receiveSize = Configuration::MAX_SIZE;
}

SelectRdtReceiver::~SelectRdtReceiver () {
	while (!waitAck.empty ()) {
		waitAck.pop ();
	}
}

void SelectRdtReceiver::receive (const struct Packet &packet) {
	int checkSum = pUtils->calculateCheckSum (packet);
	if (checkSum == packet.checksum && base <= packet.seqnum && packet.seqnum < rBase) {
		pUtils->printPacket ("接收方正确收到发送方的报文", packet);

		sort_Message msg;
		memcpy (msg.data, packet.payload, sizeof (packet.payload));
		msg.mesNum = packet.seqnum;

		waitAck.push (msg);

		while (!waitAck.empty () && waitAck.top ().mesNum == base) {
			pns->delivertoAppLayer (RECEIVER, waitAck.top ());
			while (!waitAck.empty () && waitAck.top ().mesNum == base)
				waitAck.pop ();
			base++;
		}
		rBase = min (base + receiveWindow, receiveSize);
		//pns->delivertoAppLayer(RECEIVER, msg);

		lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
		lastAckPkt.checksum = pUtils->calculateCheckSum (lastAckPkt);
		pUtils->printPacket ("接收方发送确认报文", lastAckPkt);
		pns->sendToNetworkLayer (SENDER, lastAckPkt);    //调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方

		//this->expectSequenceNumberRcvd+=1 ; //every time num will increase 1
	} else if (checkSum == packet.checksum && base - receiveWindow <= packet.seqnum && packet.seqnum < base) {
		lastAckPkt.acknum = packet.seqnum; //确认序号等于收到的报文序号
		lastAckPkt.checksum = pUtils->calculateCheckSum (lastAckPkt);
		pUtils->printPacket ("接收方发送确认报文", lastAckPkt);
		pns->sendToNetworkLayer (SENDER, lastAckPkt);    //调用模拟网络环境的sendToNetworkLayer，通过网络层发送确认报文到对方
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

