#include "dsc/plugin/dsc_service_factory.h"
#include "dsc/service/dsc_service_container.h"

#include "idc_client_service/idc_client_service.h"
#include "idc_client_service_plugin.h"
//#include "bc_comm/bc_comm_func.h"

ACE_INT32 CIdcClientServicePlugin::OnInit(void)
{
	//BC::InitOpenSsl();

	CDscServiceFactory<IDC_SDK::CIdcClientService> serviceFactory;
	ACE_INT32 nRet = ::RegistOneReactorDscService(&serviceFactory);

	if (nRet)
	{
		DSC_RUN_LOG_ERROR("regist idc client service plugin failed");
	}
	else
	{
		DSC_RUN_LOG_FINE("regist idc client service plugin successfully");
	}

	return nRet;
}

extern "C" PLUGIN_EXPORT IDscPlugin* CreateDscPlugin(void)
{
	CIdcClientServicePlugin* pPlugIn = NULL;

	DSC_NEW(pPlugIn, CIdcClientServicePlugin);

	return pPlugIn;
}
