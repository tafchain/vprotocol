#include "dsc/dispatcher/dsc_dispatcher_center.h"

#include "demo_db_service.h"
#include "demo_db_service_plugin.h"

ACE_INT32 CDemoDbServicePlugIn::OnInit(void)
{ 
	CDemoDbService* pDemoDbService;

	DSC_NEW(pDemoDbService, CDemoDbService);

	return CDscDispatcherCenterDemon::instance()->RegistDscTask(pDemoDbService, CDemoDbService::EN_SERVICE_TYPE, 1);
}

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CDemoDbServicePlugIn* pDemoDbServicePlugIn = NULL;
	
	DSC_NEW(pDemoDbServicePlugIn, CDemoDbServicePlugIn);

	return pDemoDbServicePlugIn;
}
#endif