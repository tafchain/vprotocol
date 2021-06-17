#include "dsc/dispatcher/dsc_dispatcher_center.h"

#include "demo_server_service.h"
#include "demo_server_service_factory.h"
#include "demo_server_service_plugin.h"

ACE_INT32 CDemoServerServicePlugIn::OnInit(void)
{ 
	CDemoServerService* pDemoServerService;
	
	DSC_NEW(pDemoServerService, CDemoServerService);

	ACE_INT32 nRet = CDscDispatcherCenterDemon::instance()->RegistDscTask(pDemoServerService, CDemoServerService::EN_SERVICE_TYPE, 1, true);

	if(nRet) 
	{ 
		DSC_RUN_LOG_ERROR("demo server service regist failed"); 
	} 
	else 
	{ 
		DSC_RUN_LOG_FINE("demo server service regist successfully"); 
	} 

	return nRet; 
} 

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CDemoServerServicePlugIn* pDemoServerServicePlugIn = NULL;
	
	DSC_NEW(pDemoServerServicePlugIn, CDemoServerServicePlugIn);

	return pDemoServerServicePlugIn;
}
#endif