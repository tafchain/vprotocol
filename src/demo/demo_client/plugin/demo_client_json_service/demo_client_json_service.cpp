#include "demo_client_json_service.h"

CDemoClientJsonService::CDemoClientJsonService()
	: m_nSummary(0)
{
}

ACE_INT32 CDemoClientJsonService::OnInit(void)
{
	if(CClientHttpJsonService::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo client json service init failed!");

		return -1;
	}

	this->SetDscTimer(this, 1, true);
	this->SendJsonReq();

	return 0;
}

ACE_INT32 CDemoClientJsonService::OnExit(void)
{
	this->CancelDscTimer(this);

	return CClientHttpJsonService::OnExit();
}

ACE_INT32 CDemoClientJsonService::JsonRespondMessage(NS_DEMO_MSG_DEF::CDemoJsonRspMsg& rDemoJsonRspMsg, CClientHttpReq* pClientHttpReq)
{
	++m_nSummary;
	this->SendJsonReq();

	return 0;
}

void CDemoClientJsonService::OnTimer(void)
{
	NS_DEMO_MSG_DEF::CJsonSumMsg jsonSumMsg;

	jsonSumMsg.m_nSummary = m_nSummary;
	this->SendDscMessage(jsonSumMsg);
	m_nSummary = 0;
}

void CDemoClientJsonService::SendJsonReq(void)
{
	NS_DEMO_MSG_DEF::CDemoJsonNestMsg nestMsg;
	NS_DEMO_MSG_DEF::CDemoJsonReqMsg reqMsg;

	for (int i = 1; i < 2; ++i)
	{
		nestMsg.m_lstAttr.push_back(i);
	}

	nestMsg.m_strAttr = "123";
	nestMsg.m_dtAttr.m_nYear = 2018;
	nestMsg.m_dtAttr.m_nYear = 2018;
	nestMsg.m_dtAttr.m_nMon = 10;
	nestMsg.m_dtAttr.m_nDay = 28;
	nestMsg.m_dtAttr.m_nHour = 11;
	nestMsg.m_dtAttr.m_nMin = 18;
	nestMsg.m_dtAttr.m_nSec = 30;

	for (int i = 1; i < 2; ++i)
	{
		reqMsg.m_lstDemoJsonNestMag.push_back(nestMsg);
		reqMsg.m_lstAttr.push_back(i);
	}

	reqMsg.m_demoJsonNestMag = nestMsg;

	reqMsg.m_strAttr = "123";
	reqMsg.m_dtAttr.m_nYear = 2018;
	reqMsg.m_dtAttr.m_nYear = 2018;
	reqMsg.m_dtAttr.m_nMon = 10;
	reqMsg.m_dtAttr.m_nDay = 28;
	reqMsg.m_dtAttr.m_nHour = 11;
	reqMsg.m_dtAttr.m_nMin = 18;
	reqMsg.m_dtAttr.m_nSec = 30;

	CClientHttpReq* pNewClientHttpReq = NULL;
	PROT_COMM::CDscIpAddr remoteAddr(DEMO_JSON_SERVER_IP_ADDR, DEMO_JSON_SERVER_PORT);

	DSC_THREAD_DYNAMIC_TYPE_ALLOCATE(pNewClientHttpReq);
	pNewClientHttpReq->SetURL(DSC_STRING_TYPE_PARAM(DEMO_JSON_REQ_MSG_URL));
	pNewClientHttpReq->SetKeepAlive(true);

	if (this->SendJsonRequest(reqMsg, NS_DEMO_MSG_DEF::CDemoJsonRspMsg::EN_MSG_ID, pNewClientHttpReq, remoteAddr))
	{
		DSC_RUN_LOG_WARNING("send json req failed!\n");
	}
}
