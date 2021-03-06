#ifndef COMM_ID_DEF_H_342789423809732182139785892349782317556823482318875486723456
#define COMM_ID_DEF_H_342789423809732182139785892349782317556823482318875486723456

#include "dsc/dsc_comm_def.h"

namespace NS_DEMO_ID_DEF
{
	enum
	{
		DEMO_SERVER_NODE_TYPE = 986,
		DEMO_CLIENT_NODE_TYPE = 985,
		DEMO_SERVER_NODE_ID = 1,
		DEMO_SERVER_SERVICE_TYPE = DSC::EN_MIN_USER_TASK_TYPE + 1,
		DEMO_SERVER_JSON_SERVICE_TYPE,
		DEMO_SERVER_MCP_SERVICE_TYPE,
		DEMO_DB_SERVICE_TYPE,
		DEMO_CLIENT_SERVICE_TYPE,
		DEMO_CLIENT_JSON_SERVICE_TYPE,
		DEMO_CLIENT_MCP_SERVICE_TYPE,
		DEMO_CLIENT_STATIC_SERVICE_TYPE,
		DEMO_HTTP_ACCEPTOR_TYPE,
		DEMO_MCP_ACCEPTOR_TYPE
	};

	enum
	{
		EN_DEMO_REQ_MSG = DSC::EN_MIN_USER_MSG + 1,
		EN_DEMO_RSP_MSG,
		EN_DEMO_JSON_RSP_MSG,
		EN_OPT_SUM_MSG,
		EN_JSON_SUM_MSG,
		EN_MCP_SUM_MSG
	};
}

#define DEMO_JSON_SERVER_IP_ADDR "127.0.0.1"
#define DEMO_JSON_SERVER_PORT 8888
#define DEMO_JSON_REQ_MSG_URL "/demo/demo_server/demo_json_req_message"

#define DEMO_MCP_SERVER_IP_ADDR "127.0.0.1"
#define DEMO_MCP_SERVER_PORT 9999

#endif
