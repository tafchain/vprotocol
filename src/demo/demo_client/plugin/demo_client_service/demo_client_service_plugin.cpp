#include "dsc/dispatcher/dsc_dispatcher_center.h"

#include "demo_client_service.h"
#include "demo_client_service_factory.h"
#include "demo_client_service_plugin.h"

ACE_INT32 CDemoClientServicePlugIn::OnInit(void)
{ 
	CDemoClientService* pDemoClientService;
	
	DSC_NEW(pDemoClientService, CDemoClientService);

	ACE_INT32 nRet = CDscDispatcherCenterDemon::instance()->RegistDscTask(pDemoClientService, CDemoClientService::EN_SERVICE_TYPE, 1, true);

	if(nRet) 
	{ 
		DSC_RUN_LOG_ERROR("demo client service regist failed"); 
	} 
	else 
	{ 
		DSC_RUN_LOG_FINE("demo client service regist successfully"); 
	} 

	return nRet; 
} 

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CDemoClientServicePlugIn* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CDemoClientServicePlugIn);

	return pPlugIn;
}
#endif