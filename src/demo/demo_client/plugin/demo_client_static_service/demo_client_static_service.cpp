#include "demo/demo_client/plugin/demo_client_static_service/demo_client_static_service.h"

CDemoClientStaticService::CDemoClientStaticService()
: m_nOptSummary(0)
, m_nJsonSummary(0)
, m_nMcpSummary(0)
{
}

ACE_INT32 CDemoClientStaticService::OnInit(void)
{
	if (CDscReactor::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo client static service init failed!");

		return -1;
	}

	this->SetDscTimer(this, 1, true);

	return 0;
}

ACE_INT32 CDemoClientStaticService::OnExit(void)
{
	this->CancelDscTimer(this);

	return CDscReactor::OnExit();
}

void CDemoClientStaticService::OnTimer(void)
{
	//ACE_OS::printf("opt msg sum:%d, json msg sum:%d, mcp msg sum:%d, total sum:%d!\n", m_nOptSummary, m_nJsonSummary, m_nMcpSummary, m_nOptSummary + m_nJsonSummary + m_nMcpSummary);
	ACE_OS::printf("opt msg sum:%lld!\n", m_nOptSummary);
	m_nOptSummary = 0;
	m_nJsonSummary = 0;
	m_nMcpSummary = 0;
}

void CDemoClientStaticService::OnDscMsg(NS_DEMO_MSG_DEF::COptSumMsg& rOptSumMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	m_nOptSummary += rOptSumMsg.m_nSummary;
}

void CDemoClientStaticService::OnDscMsg(NS_DEMO_MSG_DEF::CJsonSumMsg& rJsonSumMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	m_nJsonSummary += rJsonSumMsg.m_nSummary;
}

void CDemoClientStaticService::OnDscMsg(NS_DEMO_MSG_DEF::CMcpSumMsg& rMcpSumMsg, const CDscMsg::CDscMsgAddr& rSrcMsgAddr)
{
	m_nMcpSummary += rMcpSumMsg.m_nSummary;
}
