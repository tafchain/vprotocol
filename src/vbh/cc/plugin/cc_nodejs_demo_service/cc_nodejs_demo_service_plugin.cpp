#include "dsc/service/dsc_mq_service_container.h"

#include "cc_nodejs_demo_service/cc_nodejs_demo_service.h"
#include "cc_nodejs_demo_service/cc_nodejs_demo_service_factory.h"
#include "cc_nodejs_demo_service/cc_nodejs_demo_service_plugin.h"

CCcNodeJsDemoServicePlugin::CCcNodeJsDemoServicePlugin()
{
}

ACE_INT32 CCcNodeJsDemoServicePlugin::OnInit(void)
{ 
	CCcNodeJsDemoServiceFactory ccDemoServiceFactory;

	return ::RegistDscMqService(&ccDemoServiceFactory);
} 

extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CCcNodeJsDemoServicePlugin* pPlugIn = NULL;
	
	DSC_NEW(pPlugIn, CCcNodeJsDemoServicePlugin);

	return pPlugIn;
}
