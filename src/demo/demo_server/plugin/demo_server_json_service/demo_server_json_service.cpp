#include "demo_server_json_service.h"

CDemoServerJsonService::CDemoServerJsonService()
{
}

ACE_INT32 CDemoServerJsonService::OnInit(void)
{
	if(CServerHttpJsonService::OnInit())
	{
		DSC_RUN_LOG_ERROR("demo server service init failed!");

		return -1;
	}
	
	this->SetPortNo(DEMO_JSON_SERVER_PORT);

	return 0;
}

ACE_INT32 CDemoServerJsonService::JsonServiceMessage(NS_DEMO_MSG_DEF::CDemoJsonReqMsg& rDemoJsonReqMsg, const ACE_UINT32 nSessionID)
{
	NS_DEMO_MSG_DEF::CDemoJsonRspMsg rspMsg;
	NS_DEMO_MSG_DEF::CDemoJsonNestMsg nestMsg;

	for (int i = 1; i < 2; ++i)
	{
		nestMsg.m_lstAttr.push_back(i);
	}

	nestMsg.m_strAttr = "456";
	nestMsg.m_dtAttr.m_nYear = 2018;
	nestMsg.m_dtAttr.m_nYear = 2018;
	nestMsg.m_dtAttr.m_nMon = 10;
	nestMsg.m_dtAttr.m_nDay = 28;
	nestMsg.m_dtAttr.m_nHour = 11;
	nestMsg.m_dtAttr.m_nMin = 18;
	nestMsg.m_dtAttr.m_nSec = 30;

	for (int i = 1; i < 2; ++i)
	{
		rspMsg.m_lstDemoJsonNestMag.push_back(nestMsg);
		rspMsg.m_lstAttr.push_back(i);
	}

	rspMsg.m_demoJsonNestMag = nestMsg;

	rspMsg.m_strAttr = "456";
	rspMsg.m_dtAttr.m_nYear = 2018;
	rspMsg.m_dtAttr.m_nYear = 2018;
	rspMsg.m_dtAttr.m_nMon = 10;
	rspMsg.m_dtAttr.m_nDay = 28;
	rspMsg.m_dtAttr.m_nHour = 11;
	rspMsg.m_dtAttr.m_nMin = 18;
	rspMsg.m_dtAttr.m_nSec = 30;

	if (this->SendHttpJsonResponse(rspMsg, nSessionID))
	{
		DSC_RUN_LOG_WARNING("send rsp msg failed!\n");
	}

	return 0;
}

ACE_INT32 CDemoServerJsonService::OnExit(void)
{
	return CServerHttpJsonService::OnExit();
}
