#include "dsc/service/dsc_service_container.h"

#include "vbh_perf_test/reg_test_client_service_factory.h"
#include "vbh_perf_test/reg_test_client_service_plugin.h"

ACE_INT32 CRegTestServicePlugIn::OnInit(void)
{ 
	CRegTestServiceFactory factory;

	ACE_INT32 iRet = RegistReactorDscService(&factory);

	if(iRet) 
	{ 
		DSC_RUN_LOG_ERROR("reg test service factory regist failed"); 
	} 
	else 
	{ 
		DSC_RUN_LOG_FINE("reg test client service factory regist successfully"); 
	} 

	return iRet; 
} 

