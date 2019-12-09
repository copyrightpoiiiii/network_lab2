#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

struct  Configuration{

	/**
	�������Э��Payload���ݵĴ�С���ֽ�Ϊ��λ��
	*/
	static const int PAYLOAD_SIZE = 21;

	/**
	��ʱ��ʱ��
	*/
	static const int TIME_OUT =20;

	static const int WINDOW_N = 5;

};



/**
	�����Ӧ�ò����Ϣ
*/
struct  Message {
	char data[Configuration::PAYLOAD_SIZE];		//payload

	Message();
	Message(const Message &msg);
	virtual Message & operator=(const Message &msg);
	virtual ~Message();

	virtual void print();
};

/**
	���Ĳ�����㱨�Ķ�
*/
struct  Packet {
	int seqnum;										//���
	int acknum;										//ȷ�Ϻ�
	int checksum;									//У���
	char payload[Configuration::PAYLOAD_SIZE];		//payload
	
	Packet();
	Packet(const Packet& pkt);
	virtual Packet & operator=(const Packet& pkt);
	virtual bool operator==(const Packet& pkt) const;
	virtual ~Packet();

	virtual void print();
};



#endif

