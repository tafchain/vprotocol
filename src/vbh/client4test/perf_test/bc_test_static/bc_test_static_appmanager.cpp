#include "dsc/dsc_comm_def.h"

#include "vbh_test_static/vbh_test_static_appmanager.h"
#include "vbh_test_static/static_service_plugin.h"

ACE_INT32 CBcTestStaticAppManager::OnInit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

	CStaticServicePlugin staticServicePlugin;

	staticServicePlugin.OnInit();

	return 0;
}

