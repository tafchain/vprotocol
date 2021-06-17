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
	//1.��Ϣ���ͳ����������ض���app����������ϢӦ��2.���ճ�����ָ�����ŵ�ַ�����ڻ���ϢӦ��
	short m_nAppType;
	short m_nAppID;
};

class IP2pMqMsgCallback
{
public:
	virtual void OnMessage(CP2pMqAddrMsg& msg) = 0;
	virtual void Release(void) = 0;//�����˳�ǰ�ײ��ص�
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
	CP2pMqAddr m_localAddr;//�ڶ��������������У���Զ�ϵͳ����ʱ��������Ҫָ�����ص�ַ��������Ҫָ�����ص�ַ�������Ϣ���
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
	short m_nAppType;//�������ţ����Ĳ�ͬ��topic�Ľ��̹�ͬһ��
	short m_nAppID;//���̱�ţ�ĳapp type �µı�ţ�ͬһ����app��ű���Ψһ
	CP2pMqAddr m_listenAddr;//�������� ip addr
	std::list<CP2pMqConnectAddr> m_lstConnectAddr;//��Ҫ��Ϣ�����Ľ��� ip addr
	IP2pMqMsgCallback* m_pP2pMqMsgCallback;//��Ϣ�ص��ӿ��࣬�ϲ�ҵ����Ҫ�Լ�new����
};

#endif
