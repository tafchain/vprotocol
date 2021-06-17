#include "dsc/service/dsc_service_container.h"

#include "cc_demo_service/cc_demo_service.h"
#include "cc_demo_service/cc_demo_service_factory.h"
#include "cc_demo_service/cc_demo_service_plugin.h"

CCcDemoServicePlugin::CCcDemoServicePlugin()
{
}

ACE_INT32 CCcDemoServicePlugin::OnInit(void)
{ 
	CCcDemoServiceFactory ccDemoServiceFactory;

	return ::RegistReactorDscService(&ccDemoServiceFactory);
} 

#ifndef DSC_TEST
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CCcDemoServicePlugin* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CCcDemoServicePlugin);

	return pPlugIn;
}
#endif
