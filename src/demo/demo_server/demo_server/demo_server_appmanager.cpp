#include "dsc/dsc_comm_def.h"

#include "demo_server/demo_server_appmanager.h"

#ifdef _IOS_PLATFORM_
#include "demo/demo_server/plugin/demo_server_service/demo_server_service_plugin.h"
#include "demo/demo_server/plugin/demo_db_service/demo_db_service_plugin.h"
#include "demo/demo_server/plugin/demo_mcp_server_service/demo_mcp_server_service_plugin.h"
#include "demo/demo_server/plugin/demo_server_json_service/demo_server_json_service_plugin.h"
#endif

ACE_INT32 CDemoServerAppManager::OnInit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

#ifdef _IOS_PLATFORM_
	CDemoServerServicePlugIn demoServerServicePlugin;
	CDemoDbServicePlugIn  demoDbServicePlugin;
	CDemoMcpServerServicePlugIn demoMcpServerServicePlugin;
	CDemoServerJsonPlugIn demoServerJsonPlugin;

	demoServerServicePlugin.OnInit();
	demoDbServicePlugin.OnInit();
	demoMcpServerServicePlugin.OnInit();
	demoServerJsonPlugin.OnInit();
#endif
	return 0;
}

ACE_INT32 CDemoServerAppManager::OnExit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnExit());

	return 0;
}


