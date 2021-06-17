#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "mq/mq_client_sdk/mq_client_sdk_interface.h"
#include "test_mq_comm/test_mq_msg.h"

class CTestMqMsgCallback : public MQ::CMqMsgCallback
{
public:
	virtual void OnReady(void);
	virtual void OnAbnormal(void);
	virtual void OnExit(void);

public:
	BEGIN_BIND_MQ_MESSAGE
	BIND_MQ_MESSAGE(CTestMqMsgRsp1)
	BIND_MQ_MESSAGE(CTestMqMsgRsp2)
	END_BIND_MQ_MESSAGE

public:
	void OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp1& rsp);
	void OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp2& rsp);
};

unsigned int g_sendNum = 0;
unsigned int g_msgNum = 0;
bool g_bExit = false;

void SendReq1(void);
void SendReq2(void);

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("please input send msg num");

		return -1;
	}
	else
	{
		g_sendNum = atoi(argv[1]);
	}

	MQ::CInitMqClientSdkParam param;
	unsigned int nTopic[2];
	CTestMqMsgCallback * pTestMqMsgCallback = new CTestMqMsgCallback;

	nTopic[0] = CTestMqMsgRsp1::EN_TOPIC_ID;
	nTopic[1] = CTestMqMsgRsp2::EN_TOPIC_ID;

	param.m_nClientID = 1;

	pTestMqMsgCallback->Init();
	param.m_pMqMsgCallback = pTestMqMsgCallback;

	param.m_pTopicID = nTopic;
	param.m_nTopicIDNum = 2;
	InitMqClientSdk(param);
	
	for (;;)
	{
		printf("msg num:%d\n", g_msgNum);
		g_msgNum = 0;
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		for (unsigned int i = 0; i < g_sendNum; ++i)
		{
			SendReq1();
			SendReq2();
		}

		if (g_bExit)
		{
			break;
		}
	}

	return 0;
}

void SendReq1(void)
{
	CTestMqMsgReq1 testMqMsgReq1;

	testMqMsgReq1.m_nIntAttr = 123;
	MQ::PubTopicPollMqMsg(testMqMsgReq1);
}

void SendReq2(void)
{
	CTestMqMsgReq2 testMqMsgReq2;

	testMqMsgReq2.m_nIntAttr = 123;
	MQ::PubTopicPollMqMsg(testMqMsgReq2);
}

void CTestMqMsgCallback::OnReady(void)
{
	::printf("ready!\n");
	//for (unsigned int i = 0; i < g_sendNum; ++i)
	//{
	//	SendReq1();
	//	SendReq2();
	//}
}

void CTestMqMsgCallback::OnAbnormal(void)
{
	::printf("abnormal!\n");
}

void CTestMqMsgCallback::OnExit(void)
{
	g_bExit = true;
}


void CTestMqMsgCallback::OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp1& rsp)
{
	++g_msgNum;
	//SendReq1();
}

void CTestMqMsgCallback::OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp2& rsp)
{
	++g_msgNum;
	//SendReq2();
}
