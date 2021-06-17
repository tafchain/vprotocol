#ifndef DEMO_SERVER_JSON_SERVICE_H_4238234089342798435642367234987423972348234168685
#define DEMO_SERVER_JSON_SERVICE_H_4238234089342798435642367234987423972348234168685

#include "dsc/protocol/http_json/server_http_json_service.h"

#include "demo/common/comm_msg_def.h"

class PLUGIN_EXPORT CDemoServerJsonService : public CServerHttpJsonService
{
public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_SERVER_JSON_SERVICE_TYPE
	};

public:
	CDemoServerJsonService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);

public:
	BEGIN_BIND_SERVER_JSON_MESSAGE
	BIND_SERVER_JSON_MESSAGE(DEMO_JSON_REQ_MSG_URL, NS_DEMO_MSG_DEF::CDemoJsonReqMsg)
	END_BIND_SERVER_JSON_MESSAGE

public:
	ACE_INT32 JsonServiceMessage(NS_DEMO_MSG_DEF::CDemoJsonReqMsg& rDemoJsonReqMsg, const ACE_UINT32 nSessionID);
};

#endif