#include "dsc/dispatcher/dsc_dispatcher_center.h"

#include "demo/demo_client/plugin/demo_client_static_service/demo_client_static_service.h"
#include "demo/demo_client/plugin/demo_client_static_service/demo_client_static_service_plugin.h"

ACE_INT32 CDemoClientStaticServicePlugin::OnInit(void)
{ 
	CDemoClientStaticService* pService;

	DSC_NEW(pService, CDemoClientStaticService);
	
	return CDscDispatcherCenterDemon::instance()->RegistDscTask(pService, CDemoClientStaticService::EN_SERVICE_TYPE, 1);
} 

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CDemoClientStaticServicePlugin* pPlugin = NULL;
	
	DSC_NEW(pPlugin, CDemoClientStaticServicePlugin);

	return pPlugin;
}
#endif