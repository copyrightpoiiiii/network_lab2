// StopWait.cpp : 定义控制台应用程序的入口点。
//


#include "../header/Global.h"
#include "../header/RdtSender.h"
#include "../header/RdtReceiver.h"
#include "../header/StopWaitRdtSender.h"
#include "../header/StopWaitRdtReceiver.h"

/*
int main(int argc, char* argv[])
{
    RdtSender *ps = new StopWaitRdtSender();
    RdtReceiver * pr = new StopWaitRdtReceiver();
    pns->setRunMode(0);  //VERBOS模式
//	pns->setRunMode(1);  //安静模式
    pns->init();
    pns->setRtdSender(ps);
    pns->setRtdReceiver(pr);
    pns->setRtdReceiver(pr); pns->setInputFile("/home/zhangqi/CLionProjects/network_lab2/input.txt");
    pns->setOutputFile("/home/zhangqi/CLionProjects/network_lab2/output.txt");

    pns->start();

    delete ps;
    delete pr;
    delete pUtils;									//指向唯一的工具类实例，只在main函数结束前delete
    delete pns;										//指向唯一的模拟网络环境类实例，只在main函数结束前delete

    return 0;
}

*/