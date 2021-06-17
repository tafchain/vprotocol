#include "dsc/service/dsc_service_container.h"
#include "dsc/dsc_log.h"

#include "demo_client_json_service_factory.h"
#include "demo_client_json_service_plugin.h"

ACE_INT32 CDemoClientJsonPlugIn::OnInit(void)
{ 
	CDemoClientJsonFactory factory;

	ACE_INT32 iRet = RegistReactorDscService(&factory);

	if(iRet) 
	{ 
		DSC_RUN_LOG_ERROR("demo client json service regist failed"); 
	} 
	else 
	{ 
		DSC_RUN_LOG_FINE("demo client json service regist successfully"); 
	} 

	return iRet; 
} 

#ifndef _IOS_PLATFORM_
extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CDemoClientJsonPlugIn* pPlugIn = nullptr;
	
	DSC_NEW(pPlugIn, CDemoClientJsonPlugIn);

	return pPlugIn;
}
#endif