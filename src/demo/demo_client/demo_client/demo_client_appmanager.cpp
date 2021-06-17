#include "dsc/dsc_comm_def.h"

#include "demo_client/demo_client_appmanager.h"

#ifdef _IOS_PLATFORM_
#include "demo/demo_client/plugin/demo_client_json_service/demo_client_json_service_plugin.h"
#include "demo/demo_client/plugin/demo_client_service/demo_client_service_plugin.h"
#include "demo/demo_client/plugin/demo_client_static_service/demo_client_static_service_plugin.h"
#include "demo/demo_client/plugin/demo_mcp_client_service/demo_mcp_client_service_plugin.h"
#endif


ACE_INT32 CDemoClientAppManager::OnInit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

#ifdef _IOS_PLATFORM_
	CDemoClientJsonPlugIn demoClientJsonPlugin;
	CDemoClientServicePlugIn  demoClientServicePlugin;
	CDemoClientStaticServicePlugin demoClientStaticServicePlugin;
	CPerfTestServicePlugIn perfTestServicePlugin;

	demoClientJsonPlugin.OnInit();
	demoClientServicePlugin.OnInit();
	demoClientStaticServicePlugin.OnInit();
	perfTestServicePlugin.OnInit();
#endif


	return 0;
}

