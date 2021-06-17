#include "dsc/service/dsc_service_container.h"

#include "vbh_perf_test/perf_test_client_service_factory.h"
#include "vbh_perf_test/perf_test_client_service_plugin.h"

ACE_INT32 CPerfTestServicePlugIn::OnInit(void)
{ 
	CPerfTestServiceFactory factory;

	ACE_INT32 iRet = RegistReactorDscService(&factory);

	if(iRet) 
	{ 
		DSC_RUN_LOG_ERROR("perf test service factory regist failed"); 
	} 
	else 
	{ 
		DSC_RUN_LOG_FINE("perf test service factory regist successfully"); 
	} 

	return iRet; 
} 

