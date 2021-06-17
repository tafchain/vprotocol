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
	BIND_MQ_MESSAGE(CTestMqMsgReq1)
	BIND_MQ_MESSAGE(CTestMqMsgReq2)
	END_BIND_MQ_MESSAGE

public:
	void OnMqMsg(const unsigned int nClientID, CTestMqMsgReq1& req);
	void OnMqMsg(const unsigned int nClientID, CTestMqMsgReq2& req);
};

unsigned int g_msgNum = 0;
bool g_bExit = false;

int main(int argc, char* argv[])
{
	MQ::CInitMqClientSdkParam param;

	if (argc < 2)
	{
		printf("please input client id");

		return -1;
	}
	else
	{
		param.m_nClientID = atoi(argv[1]);
	}

	unsigned int nTopic[2];
	CTestMqMsgCallback * pTestMqMsgCallback = new CTestMqMsgCallback;

	nTopic[0] = CTestMqMsgReq1::EN_TOPIC_ID;
	nTopic[1] = CTestMqMsgReq2::EN_TOPIC_ID;

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

		if (g_bExit)
		{
			break;
		}
	}

	return 0;
}

void CTestMqMsgCallback::OnReady(void)
{
	::printf("ready!\n");
}

void CTestMqMsgCallback::OnAbnormal(void)
{
	::printf("abnormal!\n");
}

void CTestMqMsgCallback::OnExit(void)
{
	g_bExit = true;
}

void CTestMqMsgCallback::OnMqMsg(const unsigned int nClientID, CTestMqMsgReq1& req)
{
	++g_msgNum;

	CTestMqMsgRsp1 testMqMsgRsp1;

	testMqMsgRsp1.m_nIntAttr = 123;

	MQ::PubAddrMqMsg(nClientID, testMqMsgRsp1);
}

void CTestMqMsgCallback::OnMqMsg(const unsigned int nClientID, CTestMqMsgReq2& req)
{
	++g_msgNum;

	CTestMqMsgRsp2 testMqMsgRsp2;

	testMqMsgRsp2.m_nIntAttr = 123;

	MQ::PubAddrMqMsg(nClientID, testMqMsgRsp2);
}
