//
// Created by zhangqi on 2019/12/8.
//

#include "GoBackN_RdtSender.h"
#include "Global.h"

GoBackN_RdtSender::GoBackN_RdtSender(){
	expectSequenceNumberSend = 0;
	sendWindow = Configuration::WINDOW_N;
	rBase = Configuration::WINDOW_N;
	sendSize = Configuration::MAX_SIZE;
	base = 0;
	waitingState = false;
}

GoBackN_RdtSender::~GoBackN_RdtSender() {

}

bool GoBackN_RdtSender::getWaitingState() {
    return waitingState;
}

bool GoBackN_RdtSender::send(const Message &message) {
	if (!waitingState) {
		Packet *waitSend = new Packet;
		waitSend->acknum = -1;
		waitSend->seqnum = expectSequenceNumberSend;
		waitSend->checksum = 0;
		memcpy (waitSend->payload, message.data, sizeof (message.data));
		waitSend->checksum = pUtils->calculateCheckSum (*waitSend);
		pUtils->printPacket ("发送方发送报文", *waitSend);
		if (base == expectSequenceNumberSend) {
			pns->startTimer (SENDER, Configuration::TIME_OUT, waitSend->seqnum);            //启动发送方定时器
			pns->sendToNetworkLayer (RECEIVER, *waitSend);
		}
		std::cout << "当前滑动窗口为: " << base << " " << expectSequenceNumberSend << " " << rBase << std::endl;
		expectSequenceNumberSend++;
		if (expectSequenceNumberSend >= rBase)
			waitingState = true;
		packetWaitingAck.push_front (waitSend);
		return true;
	} else return false;
}

void GoBackN_RdtSender::receive (const struct Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum (ackPkt);
	if (checkSum == ackPkt.checksum) {
		if (!packetWaitingAck.empty () && ackPkt.acknum >= packetWaitingAck.front ()->seqnum) {
			while (!packetWaitingAck.empty () && ackPkt.acknum >= packetWaitingAck.front ()->seqnum) {
				pUtils->printPacket ("发送方正确收到确认", *packetWaitingAck.front ());
				packetWaitingAck.pop_back ();
			}
			base = ackPkt.acknum + 1;
			rBase = min (base + sendWindow, sendSize);
			if (base >= rBase)
				waitingState = true;
			else {
				waitingState = false;
				pns->startTimer (SENDER, Configuration::TIME_OUT, packetWaitingAck.front ()->seqnum);
			}
			return;
		}
	}
	Utils->printPacket ("发送方没有正确收到确认，重发上次发送的报文", this->packetWaitingAck);
	pns->stopTimer (SENDER, packetWaitingAck.front ()->seqnum);//首先关闭定时器
	for (auto pac:packetWaitingAck) {
		Utils->printPacket ("发送方没有正确收到确认，重发上次发送的报文", *pac);
		pns->sendToNetworkLayer (RECEIVER, *pac);
	}
	pns->startTimer (SENDER, Configuration::TIME_OUT, packetWaitingAck.front ()->seqnum);            //重新启动发送方定时器
}

void GoBackN_RdtSender::timeoutHandler (int seqNum) {
	pUtils->printPacket ("发送方定时器时间到，重发上次发送的报文", this->packetWaitingAck);
	pns->stopTimer (SENDER, packetWaitingAck.front ()->seqnum);//首先关闭定时器
	for (auto pac:packetWaitingAck) {
		Utils->printPacket ("发送方没有正确收到确认，重发上次发送的报文", *pac);
		pns->sendToNetworkLayer (RECEIVER, *pac);
	}
	pns->startTimer (SENDER, Configuration::TIME_OUT, packetWaitingAck.front ()->seqnum);
}