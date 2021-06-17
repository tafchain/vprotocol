#include "dsc/protocol/http/dsc_server_http_dispatcher.h"
#include "dsc/protocol/http/dsc_http_synch_acceptor.h"

#include "demo/common/comm_id_def.h"
#include "demo_server_json_service_factory.h"
#include "demo_server_json_service_plugin.h"


ACE_INT32 CDemoServerJsonPlugIn::OnInit(void)
{ 
	CDemoServerJsonFactory factory;

	::RegistServerHttpDispatcher();

	ACE_INT32 iRet = CServerHttpDispatcher::RegistNestServerHttpDscService(factory);

	if(iRet) 
	{ 
		DSC_RUN_LOG_ERROR("demoServerServiceFactory regist failed"); 
	} 
	else 
	{ 
		DSC_RUN_LOG_FINE("demoServerServiceFactory regist successfully"); 
	} 

	CDscString strIpAddr;
	ACE_INT32 nPort;
	CHttpSynchAcceptor* pHttpSynchAcceptor;

	strIpAddr = DEMO_JSON_SERVER_IP_ADDR;
	nPort = DEMO_JSON_SERVER_PORT;

	DSC_NEW(pHttpSynchAcceptor, CHttpSynchAcceptor);
	pHttpSynchAcceptor->SetType(NS_DEMO_ID_DEF::DEMO_HTTP_ACCEPTOR_TYPE);//acceptor作为一个service。必须也要有合法的type、id
	pHttpSynchAcceptor->SetID(1);
	pHttpSynchAcceptor->open(nPort, strIpAddr);

	return iRet; 
} 

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CDemoServerJsonPlugIn* pPlugIn;
	
	DSC_NEW(pPlugIn, CDemoServerJsonPlugIn);

	return pPlugIn;
}
#endif