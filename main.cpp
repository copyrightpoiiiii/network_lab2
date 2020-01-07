#include"header/Global.h"
#include"header/RdtSender.h"
#include"header/RdtReceiver.h"
#include"header/SelectRdtSender.h"
#include"header/SelectRdtReceiver.h"
#include "header/GoBackN_RdtReceiver.h"
#include "header/GoBackN_RdtSender.h"
#include "header/TcpRdtReceiver.h"
#include "header/TcpRdtSender.h"

int main(int argc, char *argv[]) {
    //如果需要使用其它的Rdt协议，只需要实例化其他具体Rdt实现类的实例， //如GBNRdtSender和GBNRdtSeceiver
    std::ofstream of("out.log");
    std::streambuf *filebuf = of.rdbuf();
    cout.rdbuf(filebuf);
    RdtSender *ps = new SelectRdtSender();
    RdtReceiver *pr = new SelectRdtReceiver();
    pns->setRunMode(0); //VERBOS模式 //pns->setRunMode(1); //安静模式
    pns->init();
    pns->setRtdSender(ps);
    pns->setRtdReceiver(pr);
    pns->setInputFile("input.txt");
    pns->setOutputFile("output.txt");
    pns->start();
    delete ps;
    delete pr;
    //指向唯一的工具类实例，只在main函数结束前delete
    delete pUtils; //指向唯一的模拟网络环境类实例，只在main函数结束前delete delete pns;
    of.flush();
    of.clear();
    return 0;
}
