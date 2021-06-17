#ifndef DEMO_MCP_SERVER_SERVICE_H_34289743219783421783217881213
#define DEMO_MCP_SERVER_SERVICE_H_34289743219783421783217881213

#include "dsc/protocol/hts/dsc_hts_service.h"

#include "demo/common/comm_msg_def.h"

class PLUGIN_EXPORT CDemoMcpServerService : public CDscHtsServerService
{
public:
	enum
	{
		EN_SERVICE_TYPE = NS_DEMO_ID_DEF::DEMO_SERVER_MCP_SERVICE_TYPE
	};

public:
	CDemoMcpServerService();

public:
	virtual ACE_INT32 OnInit(void);
	virtual ACE_INT32 OnExit(void);

protected:
	BEGIN_HTS_MESSAGE_BIND
	BIND_HTS_MESSAGE(NS_DEMO_MSG_DEF::CDemoReqMsg)
	END_HTS_MESSAGE_BIND

public:
	ACE_INT32 OnHtsMsg(NS_DEMO_MSG_DEF::CDemoReqMsg& rDemoReqMsg, CMcpHandler* pMcpHandler);
};

#endif