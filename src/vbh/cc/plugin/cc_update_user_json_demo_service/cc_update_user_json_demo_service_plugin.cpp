#include "dsc/service/dsc_service_container.h"

#include "cc_update_user_json_demo_service/cc_update_user_json_demo_service.h"
#include "cc_update_user_json_demo_service/cc_update_user_json_demo_service_factory.h"
#include "cc_update_user_json_demo_service/cc_update_user_json_demo_service_plugin.h"

CCcUpdateUserJsonDemoServicePlugin::CCcUpdateUserJsonDemoServicePlugin()
{
}

ACE_INT32 CCcUpdateUserJsonDemoServicePlugin::OnInit(void)
{ 
	CCcUpdateUserJsonDemoServiceFactory ccDemoServiceFactory;

	return ::RegistReactorDscService(&ccDemoServiceFactory);
} 

#ifndef DSC_TEST
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CCcUpdateUserJsonDemoServicePlugin* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CCcUpdateUserJsonDemoServicePlugin);

	return pPlugIn;
}
#endif
