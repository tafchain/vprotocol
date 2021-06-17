#include "demo_mcp_client_service.h"

CDemoMcpClientService::CDemoMcpClientService()
: m_nSummary(0)
{
}

ACE_INT32 CDemoMcpClientService::OnInit(void)
{
	if(CDscHtsClientService::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo mcp client service init failed!");

		return -1;
	}

	CDscString strIpAddr;

	strIpAddr = DEMO_MCP_SERVER_IP_ADDR;

	PROT_COMM::CDscIpAddr remoteAddr(strIpAddr, DEMO_MCP_SERVER_PORT);
	NS_DEMO_MSG_DEF::CDemoReqMsg reqMsg;
	
	this->InitDemoReqMsg(reqMsg);

	for (int i = 0; i < 2; ++i)
	{
		this->SendHtsMsg(reqMsg, remoteAddr);
	}

	this->SetDscTimer(this, 1, true);

	return 0;
}

ACE_INT32 CDemoMcpClientService::OnExit( void )
{
	this->CancelDscTimer(this);

	return CDscHtsClientService::OnExit();
}

ACE_INT32 CDemoMcpClientService::OnHtsMsg(NS_DEMO_MSG_DEF::CDemoRspMsg& rDemoRspMsg, CMcpHandler* pMcpHandler)
{
	NS_DEMO_MSG_DEF::CDemoReqMsg reqMsg;

	this->InitDemoReqMsg(reqMsg);
	this->SendHtsMsg(reqMsg, pMcpHandler);
	++m_nSummary;

	return 0;
}

//void CDemoMcpClientService::InitDemoReqMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& reqMsg)
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

void CDemoMcpClientService::InitDemoReqMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& reqMsg)
{
	reqMsg.m_nAttr = 123;
}

void CDemoMcpClientService::OnTimer(void)
{
	NS_DEMO_MSG_DEF::CMcpSumMsg mcpSumMsg;

	mcpSumMsg.m_nSummary = m_nSummary;
	this->SendDscMessage(mcpSumMsg);
	m_nSummary = 0;

}
