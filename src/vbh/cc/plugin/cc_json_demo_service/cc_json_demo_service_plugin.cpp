#include "dsc/service/dsc_service_container.h"

#include "cc_json_demo_service/cc_json_demo_service.h"
#include "cc_json_demo_service/cc_json_demo_service_factory.h"
#include "cc_json_demo_service/cc_json_demo_service_plugin.h"

CCcJsonDemoServicePlugin::CCcJsonDemoServicePlugin()
{
}

ACE_INT32 CCcJsonDemoServicePlugin::OnInit(void)
{ 
	CCcJsonDemoServiceFactory ccDemoServiceFactory;

	return ::RegistReactorDscService(&ccDemoServiceFactory);
} 

#ifndef DSC_TEST
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CCcJsonDemoServicePlugin* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CCcJsonDemoServicePlugin);

	return pPlugIn;
}
#endif
