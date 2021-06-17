#include "dsc/dispatcher/dsc_dispatcher_center.h"

#include "demo_client_service.h"

CDemoClientService::CDemoClientService()
: m_nSummary(0)
{
}

ACE_INT32 CDemoClientService::OnInit(void)
{
	if(CDscReactor::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo server service init failed!");

		return -1;
	}

	this->SetDscTimer(this, 1, true);


	return 0;
}

ACE_INT32 CDemoClientService::OnExit( void )
{
	this->CancelDscTimer(this);

	return CDscReactor::OnExit();
}

void CDemoClientService::OnConnected(const CDscConnectInfo& rNetInfo)
{
	CDscMsg::CDscMsgAddr addr;
	NS_DEMO_MSG_DEF::CDemoReqMsg reqMsg;
	
	this->InitDemoReqMsg(reqMsg);
	addr.SetNodeType(NS_DEMO_ID_DEF::DEMO_SERVER_NODE_TYPE);
	addr.SetNodeID(rNetInfo.m_remoteConnectInfo.m_nID);
	addr.SetServiceType(NS_DEMO_ID_DEF::DEMO_SERVER_SERVICE_TYPE);
	addr.SetServiceID(DSC::EN_INVALID_ID);

	for (int i = 0; i < 20000; ++i)
	{
		if (this->SendDscMessage(reqMsg, addr))
		{
			DSC_RUN_LOG_WARNING("send demo req mag failed!\n");
		}
	}
}

void CDemoClientService::OnPeerRegMsgType(const CDscConnectInfo& rNetInfo)
{
	NS_DEMO_MSG_DEF::CDemoReqMsg reqMsg;

	this->InitDemoReqMsg(reqMsg);

	for (int i = 0; i < 2; ++i)
	{
		if (this->SendDscMessage(reqMsg))
		{
			DSC_RUN_LOG_WARNING("send demo req mag failed!\n");
		}
	}
}

void CDemoClientService::OnDscMsg(NS_DEMO_MSG_DEF::CDemoRspMsg& rDemoRspMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	NS_DEMO_MSG_DEF::CDemoReqMsg reqMsg;

	//this->InitDemoReqMsg(reqMsg);
	reqMsg.m_nAttr = 123;
	if (this->SendDscMessage(reqMsg, rSrcMsgAddr))
	{
		DSC_RUN_LOG_WARNING("send demo req mag failed!\n");
	}

	++m_nSummary;
}

void CDemoClientService::OnTimer(void)
{
	NS_DEMO_MSG_DEF::COptSumMsg optSumMsg;

	//ACE_OS::printf("opt msg sum:%lld!\n", m_nSummary);
	optSumMsg.m_nSummary = m_nSummary;
	this->SendDscMessage(optSumMsg);
	m_nSummary = 0;
}

//void CDemoClientService::InitDemoReqMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& reqMsg)
//{
//	for (int i = 1; i < 2; ++i)
//	{
//		reqMsg.m_demoNestMag.m_lstAttr.push_back(i);
//	}
//
//	reqMsg.m_demoNestMag.m_strAttr = "123";
//	reqMsg.m_demoNestMag.m_dtAttr.m_nYear = 2018;
//	reqMsg.m_demoNestMag.m_dtAttr.m_nYear = 2018;
//	reqMsg.m_demoNestMag.m_dtAttr.m_nMon = 10;
//	reqMsg.m_demoNestMag.m_dtAttr.m_nDay = 28;
//	reqMsg.m_demoNestMag.m_dtAttr.m_nHour = 11;
//	reqMsg.m_demoNestMag.m_dtAttr.m_nMin = 18;
//	reqMsg.m_demoNestMag.m_dtAttr.m_nSec = 30;
//
//	NS_DEMO_MSG_DEF::CDemoNestMag& demoNestMag = reqMsg.m_demoNestMag;
//
//	for (int i = 1; i < 2; ++i)
//	{
//		reqMsg.m_lstdemoNestMag.push_back(demoNestMag);
//		reqMsg.m_lstAttr.push_back(i);
//	}
//
//	reqMsg.m_strAttr = "123";
//	reqMsg.m_dtAttr.m_nYear = 2018;
//	reqMsg.m_dtAttr.m_nYear = 2018;
//	reqMsg.m_dtAttr.m_nMon = 10;
//	reqMsg.m_dtAttr.m_nDay = 28;
//	reqMsg.m_dtAttr.m_nHour = 11;
//	reqMsg.m_dtAttr.m_nMin = 18;
//	reqMsg.m_dtAttr.m_nSec = 30;
//}
//
void CDemoClientService::InitDemoReqMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& reqMsg)
{
	reqMsg.m_nAttr = 123;
}

