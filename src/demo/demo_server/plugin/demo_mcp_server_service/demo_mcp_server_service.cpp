#include "demo_mcp_server_service.h"

CDemoMcpServerService::CDemoMcpServerService()
{
}

ACE_INT32 CDemoMcpServerService::OnInit(void)
{
	if(CMcpServerService::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo mcp server service init failed!");

		return -1;
	}
	else
	{
		DSC_RUN_LOG_ERROR("demo mcp server service init succeed!");
	}
	return 0;
}

//ACE_INT32 CDemoMcpServerService::OnHtsMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& rDemoReqMsg, const ACE_UINT32 nHandleID)
//{
//	NS_DEMO_MSG_DEF::CDemoRspMsg rspMsg;
//
//	rspMsg.m_demoNestMag = rDemoReqMsg.m_demoNestMag;
//	rspMsg.m_lstdemoNestMag = rDemoReqMsg.m_lstdemoNestMag;
//
//	for (int i = 1; i < 2; ++i)
//	{
//		rspMsg.m_lstAttr.push_back(i);
//	}
//
//	rspMsg.m_strAttr = "456";
//	rspMsg.m_dtAttr.m_nYear = 2018;
//	rspMsg.m_dtAttr.m_nYear = 2018;
//	rspMsg.m_dtAttr.m_nMon = 10;
//	rspMsg.m_dtAttr.m_nDay = 28;
//	rspMsg.m_dtAttr.m_nHour = 11;
//	rspMsg.m_dtAttr.m_nMin = 18;
//	rspMsg.m_dtAttr.m_nSec = 30;
//
//	if(this->SendHtsMsg(rspMsg, nHandleID))
//	{
//		DSC_RUN_LOG_INFO("send demo mcp service message failed!\n");
//	}
//
//	return 0;
//}

ACE_INT32 CDemoMcpServerService::OnHtsMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& rDemoReqMsg, CMcpHandler* pMcpHandler)
{
	NS_DEMO_MSG_DEF::CDemoRspMsg rspMsg;

	rspMsg.m_nAttr = 456;

	if (this->SendHtsMsg(rspMsg, pMcpHandler))
	{
		DSC_RUN_LOG_INFO("send demo mcp service message failed!\n");
	}

	return 0;
}

ACE_INT32 CDemoMcpServerService::OnExit( void )
{
	return CMcpServerService::OnExit();
}

