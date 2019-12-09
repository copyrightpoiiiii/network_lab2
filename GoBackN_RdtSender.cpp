//
// Created by zhangqi on 2019/12/8.
//

#include "GoBackN_RdtSender.h"
#include "Global.h"

GoBackN_RdtSender::GoBackN_RdtSender(){
    expectSequenceNumberSend=0;
    sendWindow=Configuration::WINDOW_N;
    waitingState=false;
}

GoBackN_RdtSender::~GoBackN_RdtSender() {

}

bool GoBackN_RdtSender::getWaitingState() {
    return waitingState;
}

bool GoBackN_RdtSender::send(const Message &message){

}