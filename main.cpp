#include"Global.h"
#include"RdtSender.h"
#include"RdtReceiver.h"
#include"StopWaitRdtSender.h"
#include"StopWaitRdtReceiver.h"
int main(int argc, char* argv[]) {
	//如果需要使用其它的Rdt协议，只需要实例化其他具体Rdt实现类的实例， //如GBNRdtSender和GBNRdtSeceiver
	RdtSender *ps = new StopWaitRdtSender();
	RdtReceiver * pr = new StopWaitRdtReceiver();
	pns->setRunMode(0); //VERBOS模式 //pns->setRunMode(1); //安静模式
	pns->init();
	pns->setRtdSender(ps);
	pns->setRtdReceiver(pr); pns->setInputFile("/Users/zhangqi/Downloads/input.txt ");
	pns->setOutputFile("/Users/zhangqi/Downloads/output.txt ");
	pns->start();
	delete ps;
	delete pr;
	//指向唯一的工具类实例，只在main函数结束前delete
	delete pUtils; //指向唯一的模拟网络环境类实例，只在main函数结束前delete delete pns;
	return 0;
}