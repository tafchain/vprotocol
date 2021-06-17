#ifndef MQ_IF_DEF_H_23487923487923416823167234872437
#define MQ_IF_DEF_H_23487923487923416823167234872437

class CP2pMqMsg
{
public:
	int m_nTopicID;
	void* m_pMsg;
	unsigned int m_nMsgSize;
};

class CP2pMqAddrMsg
{
public:
	CP2pMqMsg m_msg;
	//1.消息发送场景：发给特定的app，多用于消息应答；2.接收场景：指明发放地址，便于回消息应答
	short m_nAppType;
	short m_nAppID;
};

class IP2pMqMsgCallback
{
public:
	virtual void OnMessage(CP2pMqAddrMsg& msg) = 0;
	virtual void Release(void) = 0;//进程退出前底层会回调
};

class CP2pMqAddr
{
public:
	short m_nPort;
	std::string m_pIpAddr;
};

class CP2pMqConnectAddr
{
public:
	CP2pMqConnectAddr()
	{
		m_localAddr.m_nPort = -1;
	}

public:
	CP2pMqAddr m_localAddr;//在多网卡主机中运行，与对端系统连接时，往往需要指定本地地址。若不需要指定本地地址，则该信息不填。
	CP2pMqAddr m_remoteAddr;
};

class CInitMsgParam
{
public:
	CInitMsgParam()
	: m_nAppType(666)
	, m_nAppID(0)
	{

	}

public:
	short m_nAppType;//进程类别号，订阅不同组topic的进程归同一类
	short m_nAppID;//进程编号，某app type 下的编号，同一类别的app编号必须唯一
	CP2pMqAddr m_listenAddr;//进程侦听 ip addr
	std::list<CP2pMqConnectAddr> m_lstConnectAddr;//需要消息交互的进程 ip addr
	IP2pMqMsgCallback* m_pP2pMqMsgCallback;//消息回调接口类，上层业务需要自己new子类
};

#endif
