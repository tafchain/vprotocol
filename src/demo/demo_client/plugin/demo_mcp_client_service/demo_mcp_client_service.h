#ifndef DEMO_MCP_CLIENT_SERVICE_H_342842387934297823167321
#define DEMO_MCP_CLIENT_SERVICE_H_342842387934297823167321

#include "dsc/protocol/hts/dsc_hts_service.h"
#include "dsc/service_timer/dsc_service_timer_handler.h"

#include "demo/common/comm_msg_def.h"

class PLUGIN_EXPORT CDemoMcpClientService : public CDscHtsClientService, public CDscServiceTimerHandler
{
public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_CLIENT_MCP_SERVICE_TYPE
	};

public:
	CDemoMcpClientService();

public:
	ACE_INT32 OnInit(void);
	ACE_INT32 OnExit(void);
	void OnTimer(void);

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(NS_DEMO_MSG_DEF::CDemoRspMsg)
	END_HTS_MESSAGE_BIND

public:
	ACE_INT32 OnHtsMsg(NS_DEMO_MSG_DEF::CDemoRspMsg& rDemoRspMsg, CMcpHandler* pMcpHandler);

private:
	void InitDemoReqMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& reqMsg);

private:
	ACE_INT32 m_nSummary;
};

#endif
