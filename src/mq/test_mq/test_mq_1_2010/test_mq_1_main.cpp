#include <time.h>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "test_mq_comm/old_test_mq_msg.h"
#include "mq/mq_client_sdk/mq_client_sdk_interface.h"

class CTestMqMsgCallback : public MQ::CMqMsgCallback
{
public:
	virtual void OnReady(void);

public:
	BEGIN_BIND_MQ_MESSAGE
	BIND_MQ_MESSAGE(CTestMqMsgRsp1)
	BIND_MQ_MESSAGE(CTestMqMsgRsp2)
	END_BIND_MQ_MESSAGE

public:
	void OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp1& rsp);
	void OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp2& rsp);
};

unsigned int g_msgNum = 0;
int main(int argc, char* argv[])
{
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
	}

	return 0;
}

void SendReq1(void)
{
	CTestNestMqMsg testNextMqMsg;
	CTestMqMsgReq1 testMqMsgReq1;

	testMqMsgReq1.m_nIntAttr = 123;
	testMqMsgReq1.m_strAttr = "456";
	CODEC::CCodecDateTime::GetDateTimeFromSecs(testMqMsgReq1.m_datetimeAttr, time(0));
	*testMqMsgReq1.m_optAttrl = "789";
	testMqMsgReq1.m_optAttrl.Exist(true);

	testNextMqMsg.m_nIntAttr = 123;
	testNextMqMsg.m_strAttr = "456";
	CODEC::CCodecDateTime::GetDateTimeFromSecs(testNextMqMsg.m_datetimeAttr, time(0));
	*testNextMqMsg.m_optAttrl = "789";
	testNextMqMsg.m_optAttrl.Exist(true);

	testMqMsgReq1.m_lstAttr.push_back(testNextMqMsg);

	MQ::PubTopicPollMqMsg(testMqMsgReq1);
}

void SendReq2(void)
{
	CTestNestMqMsg testNextMqMsg;
	CTestMqMsgReq2 testMqMsgReq2;

	testNextMqMsg.m_nIntAttr = 123;
	testNextMqMsg.m_strAttr = "456";
	CODEC::CCodecDateTime::GetDateTimeFromSecs(testNextMqMsg.m_datetimeAttr, time(0));
	*testNextMqMsg.m_optAttrl = "789";
	testNextMqMsg.m_optAttrl.Exist(true);

	testMqMsgReq2.m_nIntAttr = 123;
	testMqMsgReq2.m_strAttr = "456";
	CODEC::CCodecDateTime::GetDateTimeFromSecs(testMqMsgReq2.m_datetimeAttr, time(0));
	*testMqMsgReq2.m_optAttrl = "789";
	testMqMsgReq2.m_optAttrl.Exist(true);
	testMqMsgReq2.m_lstAttr.push_back(testNextMqMsg);

	MQ::PubTopicPollMqMsg(testMqMsgReq2);
}

void CTestMqMsgCallback::OnReady(void)
{
	SendReq1();
	SendReq2();
}


void CTestMqMsgCallback::OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp1& rsp)
{
	++g_msgNum;
	SendReq1();
}

void CTestMqMsgCallback::OnMqMsg(const unsigned int nClientID, CTestMqMsgRsp2& rsp)
{
	++g_msgNum;
	SendReq2();
}
