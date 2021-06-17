#ifndef DEMO_CLIENT_JSON_SERVICE_H_4238794230982347634293427834789
#define DEMO_CLIENT_JSON_SERVICE_H_4238794230982347634293427834789

#include "dsc/protocol/http_json/client_http_json_service.h"
#include "dsc/service_timer/dsc_service_timer_handler.h"

#include "demo/common/comm_msg_def.h"

class PLUGIN_EXPORT CDemoClientJsonService : public CClientHttpJsonService, public CDscServiceTimerHandler
{
public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_CLIENT_JSON_SERVICE_TYPE
	};

public:
	CDemoClientJsonService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);

public:
	BEGIN_BIND_CLIENT_JSON_RESPOND
	BIND_CLIENT_JSON_MESSAGE(NS_DEMO_MSG_DEF::CDemoJsonRspMsg)
	END_BIND_CLIENT_JSON_RESPOND

	void OnTimer(void);

public:
	ACE_INT32 JsonRespondMessage(NS_DEMO_MSG_DEF::CDemoJsonRspMsg& rDemoJsonRspMsg, CClientHttpReq* pClientHttpReq);

private:
	void SendJsonReq(void);

private:
	ACE_INT32 m_nSummary;
};

#endif