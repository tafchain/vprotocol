#include "dsc/dsc_comm_def.h"

#include "common/sdk_appmanager.h"
#include "common/vbh_comm_func.h"
#include "common/vbh_encrypt_lib.h"

#ifdef _IOS_PLATFORM_
#include "sdk_client_service/sdk_client_service_plugin.h"
#endif

ACE_INT32 CSdkAppManager::OnInit()
{
	VBH::InitOpenSsl();
	DSC_FORWARD_CALL(CDscAppManager::OnInit() );

#ifdef _IOS_PLATFORM_
	CSdkClientServicePlugIn sdkClientServicePlugin;

	sdkClientServicePlugin.OnInit();
#endif

	return 0;
}

ACE_INT32 CSdkAppManager::OnExit()
{
	DSC_FORWARD_CALL(CDscAppManager::OnExit());

	return 0;
}


