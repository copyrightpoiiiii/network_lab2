//
// Created by zhangqi on 2019/12/8.
//

#include "../header/TcpRdtSender.h"
#include "../header/Global.h"

TcpRdtSender::TcpRdtSender () {
	expectSequenceNumberSend = 0;
	sendWindow = Configuration::WINDOW_N;
	rBase = Configuration::WINDOW_N;
	sendSize = Configuration::MAX_SIZE;
	base = 0;
	waitingState = false;
	count = 0;
}

TcpRdtSender::~TcpRdtSender () {
	while (!packetWaitingAck.empty ()) {
		delete packetWaitingAck.front ();
		packetWaitingAck.pop_front ();
	}
}

bool TcpRdtSender::getWaitingState () {
	return waitingState;
}

bool TcpRdtSender::send (const Message &message) {
	std::cout << waitingState << std::endl;
	if (!waitingState) {
        sort_Packet_tcp *waitSend = new sort_Packet_tcp;
        waitSend->acknum = -1;
        waitSend->seqnum = expectSequenceNumberSend;
        waitSend->checksum = 0;
        waitSend->acked = false;
        memcpy(waitSend->payload, message.data, sizeof(message.data));
        waitSend->checksum = pUtils->calculateCheckSum(*waitSend);
        pUtils->printPacket("发送方发送报文", *waitSend);
        pns->startTimer(SENDER, Configuration::TIME_OUT, waitSend->seqnum);            //启动发送方定时器
        pns->sendToNetworkLayer(RECEIVER, *waitSend);
        std::cout << "当前滑动窗口为: " << base << " " << expectSequenceNumberSend << " " << rBase << std::endl;
		expectSequenceNumberSend++;
		if (expectSequenceNumberSend >= rBase)
			waitingState = true;
		packetWaitingAck.push_back (waitSend);
		return true;
	} else return false;
}

void TcpRdtSender::receive (const Packet &ackPkt) {
	int checkSum = pUtils->calculateCheckSum (ackPkt);
	if (checkSum == ackPkt.checksum) {
		if (!packetWaitingAck.empty () && ackPkt.acknum >= base && ackPkt.acknum < expectSequenceNumberSend) {
            for (auto pac:packetWaitingAck)
                if (pac->seqnum == ackPkt.acknum && (!pac->acked)) {
                    count = 0;
                    pUtils->printPacket("发送方正确收到确认", *pac);
                    pns->stopTimer(SENDER, ackPkt.acknum);
                    pac->acked = true;
                    while ((!packetWaitingAck.empty()) && packetWaitingAck.front()->acked) {
                        base = packetWaitingAck.front()->seqnum + 1;
                        rBase = min(base + sendWindow, sendSize);
                        std::cout << "now window: " << base << " " << expectSequenceNumberSend << " " << rBase
                                  << std::endl;
                        waitingState = base >= rBase;
                        delete packetWaitingAck.front();
                        packetWaitingAck.pop_front();
                    }
                    break;
                } else if (pac->seqnum == ackPkt.acknum) {
                    count++;
                }
        } else count++;
        if (count >= 2) {
            if (packetWaitingAck.size() > 0) {
                std::cout << base << " " << expectSequenceNumberSend << " " << rBase << std::endl;
                std::cout << "收到3个冗余ACK，重发上次发送的报文 " << packetWaitingAck.front()->seqnum << std::endl;
                pUtils->printPacket("收到3个冗余ACK，重发上次发送的报文", *packetWaitingAck.front());
                pns->stopTimer(SENDER, packetWaitingAck.front()->seqnum);//首先关闭定时器
                pns->sendToNetworkLayer(RECEIVER, *packetWaitingAck.front());
                pns->startTimer(SENDER, Configuration::TIME_OUT, packetWaitingAck.front()->seqnum);//重新启动发送方定时器
            }
            count = 0;
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

void TcpRdtSender::timeoutHandler (int seqNum) {
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