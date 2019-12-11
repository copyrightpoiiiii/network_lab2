//
// Created by zhangqi on 2019/12/8.
//

#include "SelectRdtSender.h"
#include "../header/Global.h"

SelectRdtSender::SelectRdtSender () {
	expectSequenceNumberSend = 0;
	sendWindow = Configuration::WINDOW_N;
	rBase = Configuration::WINDOW_N;
	sendSize = Configuration::MAX_SIZE;
	base = 0;
	waitingState = false;
}

SelectRdtSender::~SelectRdtSender () {
	while (!packetWaitingAck.empty ()) {
		delete packetWaitingAck.front ();
		packetWaitingAck.pop_front ();
	}
}

bool SelectRdtSender::getWaitingState () {
	return waitingState;
}

bool SelectRdtSender::send (const Packet &message) {
	std::cout << waitingState << std::endl;
	if (!waitingState) {
		sort_Packet *waitSend = new sort_Packet;
		waitSend->acknum = -1;
		waitSend->seqnum = expectSequenceNumberSend;
		waitSend->checksum = 0;
		memcpy (waitSend->payload, message.data, sizeof (message.data));
		waitSend->checksum = pUtils->calculateCheckSum (*waitSend);
		pUtils->printPacket ("发送方发送报文", *waitSend);
		pns->startTimer (SENDER, Configuration::TIME_OUT, waitSend->seqnum);            //启动发送方定时器
		pns->sendToNetworkLayer (RECEIVER, *waitSend);
		std::cout << "当前滑动窗口为: " << base << " " << expectSequenceNumberSend << " " << rBase << std::endl;
		expectSequenceNumberSend++;
		if (expectSequenceNumberSend >= rBase)
			waitingState = true;
		packetWaitingAck.push_back (waitSend);
		return true;
	} else return false;
}

bool SelectRdtSender::receive (const Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum (ackPkt);
	if (checkSum == ackPkt.checksum) {
		if (!packetWaitingAck.empty () && ackPkt.acknum >= base && ackPkt.acknum < expectSequenceNumberSend) {
			for (auto pac:packetWaitingAck)
				if (pac->seqnum == ackPkt.acknum) {
					pUtils->printPacket ("发送方正确收到确认", *pac);
					pns->stopTimer (SENDER, ackPkt.acknum);
					pac->acked = true;
					break;
				}
			while (!packetWaitingAck.empty () && packetWaitingAck.front ()->acked) {
				base = packetWaitingAck.front ()->seqnum + 1;
				delete waitAckPkt.front ();
				packetWaitingAck.pop_front ();
			}
			rBase = min (base + sendWindow, sendSize);
			std::cout << "now window: " << base << " " << expectSequenceNumberSend << " " << rBase << std::endl;
			if (base >= rBase)
				waitingState = true;
			else {
				waitingState = false;
			}
			return;
		}
	}
	/*if(!packetWaitingAck.empty()) {
		pUtils->printPacket("发送方没有正确收到确认，重发上次发送的报文", *packetWaitingAck.front());
		pns->stopTimer(SENDER, packetWaitingAck.front()->seqnum);//首先关闭定时器
		for (auto pac:packetWaitingAck) {
			pUtils->printPacket("发送方没有正确收到确认，重发上次发送的报文", *pac);
			pns->sendToNetworkLayer(RECEIVER, *pac);
		}
		pns->startTimer(SENDER, Configuration::TIME_OUT, packetWaitingAck.front()->seqnum);            //重新启动发送方定时器
	}
	 */
}

void SelectRdtSender::timeoutHandler (int seqNum) {
	if (!packetWaitingAck.empty ()) {
		for (auto pac:packetWaitingAck)
			if (pac->seqnum == seqNum) {
				pns->stopTimer (SENDER, seqNum);//首先关闭定时器
				pUtils->printPacket ("发送方没有正确收到确认，重发上次发送的报文", *pac);
				pns->sendToNetworkLayer (RECEIVER, *pac);
				pns->startTimer (SENDER, Configuration::TIME_OUT, seqNum);
				break;
			}
	}
}