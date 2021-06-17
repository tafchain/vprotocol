#include "dsc/service/dsc_service_container.h"
#include "dsc/dsc_log.h"

#include "demo_mcp_client_service_factory.h"
#include "demo_mcp_client_service_plugin.h"

ACE_INT32 CPerfTestServicePlugIn::OnInit(void)
{ 
	CPerfTestServiceFactory factory;

	ACE_INT32 iRet = RegistReactorDscService(&factory);

	if(iRet) 
	{ 
		DSC_RUN_LOG_ERROR("demo mcp client service factory regist failed"); 
	} 
	else 
	{ 
		DSC_RUN_LOG_FINE("demo mcp client service factory regist successfully"); 
	} 

	return iRet; 
} 

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CPerfTestServicePlugIn* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CPerfTestServicePlugIn);

	return pPlugIn;
}
#endif