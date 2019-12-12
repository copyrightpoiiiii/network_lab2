//
// Created by zhangqi on 2019/12/8.
//

#include "../header/GoBackN_RdtSender.h"
#include "../header/Global.h"

GoBackN_RdtSender::GoBackN_RdtSender(){
	expectSequenceNumberSend = 0;
	sendWindow = Configuration::WINDOW_N;
	rBase = Configuration::WINDOW_N / 2;
	sendSize = Configuration::MAX_SIZE;
	base = 0;
	waitingState = false;
}

GoBackN_RdtSender::~GoBackN_RdtSender () {
	while (!packetWaitingAck.empty ()) {
		delete packetWaitingAck.front ();
		packetWaitingAck.pop_front ();
	}
}

bool GoBackN_RdtSender::getWaitingState () {
	return waitingState;
}

bool GoBackN_RdtSender::inWindow (int pos) {
	int l = (pos - base + sendWindow) % sendWindow;
	int r = (rBase - base + sendWindow) % sendWindow;
	return l < r;
}

bool GoBackN_RdtSender::send (const Message &message) {
	std::cout << waitingState << std::endl;
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
		expectSequenceNumberSend %= sendWindow;
		if (!inWindow (expectSequenceNumberSend))
			waitingState = true;
		packetWaitingAck.push_back (waitSend);
		return true;
	} else return false;
}

void GoBackN_RdtSender::receive (const struct Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum (ackPkt);
	if (checkSum == ackPkt.checksum) {
		if (!packetWaitingAck.empty () && inWindow (ackPkt.acknum)) {
			if (ackPkt.acknum == base)
				pns->stopTimer (SENDER, packetWaitingAck.front ()->seqnum);
			while (!packetWaitingAck.empty () && inWindow (ackPkt.acknum)) {
				pUtils->printPacket ("发送方正确收到确认", *packetWaitingAck.front ());
				delete packetWaitingAck.front ();
				base=(base+1)%sendWindow;
				rBase = (base + sendWindow/2)%sendWindow;
				packetWaitingAck.pop_front ();
			}
            std::cout << "now window: " << base << " " << expectSequenceNumberSend << " " << rBase << std::endl;
			waitingState = false;
			if (!packetWaitingAck.empty ())
			    pns->startTimer (SENDER, Configuration::TIME_OUT, packetWaitingAck.front ()->seqnum);
			return;
		}
	}
	if(!packetWaitingAck.empty()) {
        pUtils->printPacket("发送方没有正确收到确认，重发上次发送的报文", *packetWaitingAck.front());
        pns->stopTimer(SENDER, packetWaitingAck.front()->seqnum);//首先关闭定时器
        for (auto pac:packetWaitingAck) {
            pUtils->printPacket("发送方没有正确收到确认，重发上次发送的报文", *pac);
            pns->sendToNetworkLayer(RECEIVER, *pac);
        }
        pns->startTimer(SENDER, Configuration::TIME_OUT, packetWaitingAck.front()->seqnum);            //重新启动发送方定时器
    }
}

void GoBackN_RdtSender::timeoutHandler (int seqNum) {
    if(!packetWaitingAck.empty()) {
        pns->stopTimer(SENDER, packetWaitingAck.front()->seqnum);//首先关闭定时器
        for (auto pac:packetWaitingAck) {
            pUtils->printPacket("发送方没有正确收到确认，重发上次发送的报文", *pac);
            pns->sendToNetworkLayer(RECEIVER, *pac);
        }
        pns->startTimer(SENDER, Configuration::TIME_OUT, packetWaitingAck.front()->seqnum);
    }
}