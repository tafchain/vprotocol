#include "dsc/service/dsc_service_container.h"
#include "dsc/protocol/mcp/mcp_synch_acceptor.h"

#include "demo_mcp_server_service_plugin.h"
#include "demo_mcp_server_service_factory.h"
#include "demo_mcp_server_service.h"

CDemoMcpServerServicePlugIn::CDemoMcpServerServicePlugIn()
{
}

ACE_INT32 CDemoMcpServerServicePlugIn::OnInit(void)
{ 
	CDemoMcpServerServiceFactory factory;

	ACE_INT32 iRet = RegistReactorDscService(&factory);

	if(iRet) 
	{ 
		DSC_RUN_LOG_ERROR("demo mcp server service regist failed"); 
	} 
	else 
	{ 
		CDscString strCfg;
		CMcpSynchAcceptor* pMcpSynchAcceptor;

		strCfg = DEMO_MCP_SERVER_IP_ADDR;
		DSC_NEW(pMcpSynchAcceptor, CMcpSynchAcceptor(CDemoMcpServerService::EN_SERVICE_TYPE));
		pMcpSynchAcceptor->SetType(NS_DEMO_ID_DEF::DEMO_MCP_ACCEPTOR_TYPE);
		pMcpSynchAcceptor->SetID(1);
		pMcpSynchAcceptor->open(DEMO_MCP_SERVER_PORT, strCfg);
		DSC_RUN_LOG_FINE("demo mcp server service regist successfully");
	} 

	return iRet; 
} 

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CDemoMcpServerServicePlugIn* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CDemoMcpServerServicePlugIn);

	return pPlugIn;
}
#endif