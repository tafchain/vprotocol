#include "dsc/service/dsc_service_container.h"

#include "cc_update_user_demo_service/cc_update_user_demo_service.h"
#include "cc_update_user_demo_service/cc_update_user_demo_service_factory.h"
#include "cc_update_user_demo_service/cc_update_user_demo_service_plugin.h"

CCcUpdateUserDemoServicePlugin::CCcUpdateUserDemoServicePlugin()
{
}

ACE_INT32 CCcUpdateUserDemoServicePlugin::OnInit(void)
{ 
	CCcUpdateUserDemoServiceFactory ccDemoServiceFactory;

	return ::RegistReactorDscService(&ccDemoServiceFactory);
} 

#ifndef DSC_TEST
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CCcUpdateUserDemoServicePlugin* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CCcUpdateUserDemoServicePlugin);

	return pPlugIn;
}
#endif
